#include "convexgeometryintersectionrenderer.h"
#include "Rendering/Renderers/ModelMainRenderer/mainrendererconfig.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryparser.h"
#include "Utils/shaderutils.h"
#include "Utils/openglutils.h"
#include "Utils/polyhedronutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Common/Constants.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Vertex.h"
#include "Model/PolygonMesh.h"
#include "SelectionStrategies/Selection.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS_ATTRIBUTE 1
ConvexGeometryIntersectionRenderer::ConvexGeometryIntersectionRenderer():
	Renderer("Geometry Intersection"),SelectionStrategy("Geometry Intersection"){
	ok = true;
	configRenderer = (ConvexGeometryIntersectionRendererConfig*)0;
	configSelection = (ConvexGeometryIntersectionConfigSelection*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+4.0f;

}
ConvexGeometryIntersectionRenderer::~ConvexGeometryIntersectionRenderer(){
	if(configRenderer)
		delete configRenderer;
	if(configSelection)
		delete configSelection;
}

void ConvexGeometryIntersectionRenderer::glewIsReadyRenderer(){
	configSelection = new ConvexGeometryIntersectionConfigSelection();
	configRenderer = new ConvexGeometryIntersectionRendererConfig();
	createMainShader();
	if(ok)
		this->applyConfigChanges();
}

void ConvexGeometryIntersectionRenderer::draw(RModel* rmodel){
	drawWithMainShader(rmodel);
	drawWithConvexGeometryShader(rmodel);
}

void ConvexGeometryIntersectionRenderer::drawWithMainShader(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	glUseProgram(theProgram);
	//default
	ShaderUtils::setUniform(theProgram, "isSphere",0);
	ShaderUtils::setUniform(theProgram, "nPlanes",0);
	convexGeometry->setUniformData(theProgram);
	ShaderUtils::setUniform(theProgram, "drawOutsideElements", configRenderer->drawOutsideElements);
	ShaderUtils::setUniform(theProgram, "ousideElementBaseColor", configRenderer->outsideColor);
	ShaderUtils::setUniform(theProgram, "ousideElementSelectedColor", configRenderer->outsideSelectedColor);
	ShaderUtils::setUniform(theProgram, "invertInside",configRenderer->invertIntersection);
	ShaderUtils::setUniform(theProgram, "realTimeCalc",!configRenderer->followModel);
	ShaderUtils::setUniform(theProgram, "P",rmodel->getProjectionMatrix());
	ShaderUtils::setUniform(theProgram, "MV",rmodel->getMV());
	ShaderUtils::setUniform(theProgram, "NormalMatrix",glm::mat3(rmodel->getMV()));
	ShaderUtils::setUniform(theProgram, "ViewportMatrix",rmodel->getViewPortMatrix());
	ShaderUtils::setUniform(theProgram, "BorderColor",this->configRenderer->borderCutColor);
	ShaderUtils::setUniform(theProgram, "Line.Width",this->configRenderer->mainRendererConfig->wireFrameLineWidthM);
	ShaderUtils::setUniform(theProgram, "Line.Color",this->configRenderer->mainRendererConfig->wireFrameColors);
	ShaderUtils::setUniform(theProgram, "Line.isDrawn",this->configRenderer->mainRendererConfig->drawWireFrame);
	ShaderUtils::setUniform(theProgram, "ModelBaseColor",this->configRenderer->mainRendererConfig->baseModelColors);
	ShaderUtils::setUniform(theProgram, "ElementDrawOption",this->configRenderer->mainRendererConfig->elementDrawnOption);
	ShaderUtils::setUniform(theProgram, "SelectedElementColor",this->configRenderer->mainRendererConfig->selectedElementColors);
	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindTexture(GL_TEXTURE_1D, 0);
	glUseProgram(0);

}
bool ConvexGeometryIntersectionRenderer::rmodelChanged(RModel* rmodel){
	if(!rmodel->getOriginalModel() ||
			rmodel->getModelType()==vis::CONSTANTS::VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::NO_MODEL)
		return false;
	this->rmodel = rmodel;
	configRenderer->loadedModelIsLightWeight(rmodel->getOriginalModel()->isLightWeight());
	applyConfigChanges(rmodel);
	return true;
}

void ConvexGeometryIntersectionRenderer::drawWithConvexGeometryShader(RModel* rmodel){
	if(!convexGeometry)
		return;
	if(configRenderer->followModel)
		convexGeometry->render(rmodel->getMVP());
	else
		convexGeometry->render(rmodel->getProjectionMatrix()*lastMVMatrix);
}

BaseRendererConfig* ConvexGeometryIntersectionRenderer::getRendererConfigWidget(){
	return this->configRenderer;
}

bool ConvexGeometryIntersectionRenderer::hasRendererConfigWidget(){
	return true;
}
void ConvexGeometryIntersectionRenderer::applyConfigChanges(RModel * rmodel){
	this->configRenderer->readConfig();
	if(rmodel){
		if(configRenderer->changedFollowModel)
			lastMVMatrix = rmodel->getMV();
		if(configRenderer->resetPosition && !configRenderer->followModel)
			lastMVMatrix = rmodel->getModelToCameraSpaceAndScaleMatrix();
	}else
		lastMVMatrix = glm::mat4();
	parseNewGeometryString(rmodel);
}

void ConvexGeometryIntersectionRenderer::createMainShader(){
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgirmm.vert");
	vertexShaderData.addFile("Rendering/Renderers/ConvexGeometryIntersectionRenderer/intersectionFunctions.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgirmm.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgirmm.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData selectAttr = {VERTEX_FLAGS_ATTRIBUTE, "VertexFlag"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(selectAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
}

void ConvexGeometryIntersectionRenderer::parseNewGeometryString(RModel * rmodel){
	//try{
	if(configRenderer->geometryTextChanged)
		convexGeometry = ConvexGeometryParser::getConvexGeometry(configRenderer->geometryText, configRenderer);
	configRenderer->geometryTextChanged = false;
	convexGeometry->loadDataToGPU(lastMVMatrix);
	if(configRenderer->followModel)
		createInsideAttributeBufferObject(rmodel);
	//if(valid)//fail
	//}catch()
}

bool ConvexGeometryIntersectionRenderer::createInsideAttributeBufferObject(RModel * rmodel){
	if(!rmodel||!rmodel->getOriginalModel() || !configRenderer->followModel)
		return true;
	std::vector<RVertexFlagAttribute>& inside = rmodel->vertexFlagsAttribute;
	convexGeometry->move(rmodel->getMV());
	VertexCloud* model = (VertexCloud*)rmodel->getOriginalModel();
	std::vector<vis::Vertex*>& polygons = model->getVertices();
	for( vis::Vertex* vertex : polygons ){
		glm::vec4 newPosition = rmodel->getMV()*glm::vec4(vertex->getGeometricCenter(),1.0f);
		newPosition/=newPosition.w;
		bool in = convexGeometry->isPointInside(glm::vec3(newPosition));
		if(configRenderer->invertIntersection)
			in = !in;
		std::vector<int>& rmodelPos = vertex->getRmodelPositions();
		for( int position : rmodelPos){
			if(in)
				inside[position].enableFlag(RVertexFlagAttribute::INSIDE_CONVEX_GEOMETRY);
			else
				inside[position].disableFlag(RVertexFlagAttribute::INSIDE_CONVEX_GEOMETRY);

		}
	}
	rmodel->refreshSelectedElements();
	return true;
}


////SELECTION STRATEGY METHODS
bool ConvexGeometryIntersectionRenderer::hasConfigWidget(){
	return true;
}

BaseRendererConfig* ConvexGeometryIntersectionRenderer::getConfigWidget(){
	return configSelection;
}

bool ConvexGeometryIntersectionRenderer::selectElement( vis::Polygon *e, Selection* s){
	return selectElement(e,vis::CONSTANTS::POLYGON,s);
}
bool ConvexGeometryIntersectionRenderer::selectElement( vis::Polyhedron *e, Selection* s){
	return selectElement(e,vis::CONSTANTS::POLYHEDRON,s);
}
bool ConvexGeometryIntersectionRenderer::selectElement( vis::Element * e,
														int elementType,
														Selection* s){
	if(ConvexGeometryIntersectionConfigSelection::NEW_SELECTION == configSelection->selectionType){
		if(selectElementAlgorithm(e, elementType)){
			s->addSelectedElement(e);
			return true;
		}
		if(e->isSelected())
			s->removeSelectedElement(e);
		return false;
	}
	if(ConvexGeometryIntersectionConfigSelection::JOIN_SELECTION == configSelection->selectionType){
		if(e->isSelected())
			return true;
		if(selectElementAlgorithm(e, elementType)){
			s->addSelectedElement(e);
			return true;
		}
		return false;
	}
	if(ConvexGeometryIntersectionConfigSelection::INTERSECT_SELECTION == configSelection->selectionType){
		if(!e->isSelected())
			return false;
		if(selectElementAlgorithm(e, elementType))
			return true;
		if(elementType==vis::CONSTANTS::POLYHEDRON)
			s->removeSelectedElement((vis::Polyhedron*)e);
		else
			s->removeSelectedElement(e);
		return false;
	}
	if(ConvexGeometryIntersectionConfigSelection::INVERT_SELECTION == configSelection->selectionType){
		if(selectElementAlgorithm(e, elementType)){
			if(e->isSelected()){
				if(elementType==vis::CONSTANTS::POLYHEDRON)
					s->removeSelectedElement((vis::Polyhedron*)e);
				else
					s->removeSelectedElement(e);
				return false;
			}
			s->addSelectedElement(e);
			return true;
		}
		return e->isSelected();
	}
	return false;
}
bool ConvexGeometryIntersectionRenderer::selectElementAlgorithm( vis::Element * e, int elementType){
	if(configRenderer->followModel)
		return ConvexGeometryIntersectionRenderer::selectElementFollowModel(e);
	switch(elementType){//not following
		case vis::CONSTANTS::POLYGON:{
			if(gpuSelectionAlgorithmAvailable())
				//TODO call GPU Algorithm
				return false;
			//NO GPU algorithm available
			return selectCPU((vis::Polygon*)e);
		}
		case vis::CONSTANTS::POLYHEDRON:{
			if(gpuSelectionAlgorithmAvailable())
				//TODO call GPU Algorithm
				return false;
			//NO GPU algorithm available
			return selectCPU((vis::Polyhedron*)e);
		}
		case vis::CONSTANTS::VERTEX:{
			if(gpuSelectionAlgorithmAvailable())
				//TODO call GPU Algorithm
				return false;
			//NO GPU algorithm available
			return selectCPU((vis::Vertex*)e);
		}
		default: return false;
	}
}
bool ConvexGeometryIntersectionRenderer::selectElementFollowModel( vis::Element * e){
	//all vertices must be inside the convex geometry
	std::vector<int>& rmodelPos = e->getRmodelPositions();
	std::vector<RVertexFlagAttribute>& rvertexflags = rmodel->vertexFlagsAttribute;
	for( int position : rmodelPos )
		if(!rvertexflags[position].
				isFlagEnabled(RVertexFlagAttribute::INSIDE_CONVEX_GEOMETRY))
			return false;
	return true;
}

bool ConvexGeometryIntersectionRenderer::selectCPU(vis::Polyhedron* e){
	std::vector<vis::Vertex*> vertices;
	PolyhedronUtils::getPolyhedronVertices(e,vertices);
	for( vis::Vertex* vertex : vertices )
		if(!selectCPU(vertex))
			return false;
	return true;
}

bool ConvexGeometryIntersectionRenderer::selectCPU(vis::Polygon* e){
	std::vector<vis::Vertex*>& vertices = e->getVertices();
	for( vis::Vertex* vertex : vertices )
		if(!selectCPU(vertex))
			return false;
	return true;
}

bool ConvexGeometryIntersectionRenderer:: selectCPU(vis::Vertex* e){
	glm::vec4 newPosition = rmodel->getMV()*glm::vec4(e->getCoords(),1.0f);
	newPosition/=newPosition.w;
	return convexGeometry->isPointInside(glm::vec3(newPosition));
}

bool ConvexGeometryIntersectionRenderer::setupPreApplying(Selection* sel, RModel *){
	if(!convexGeometry)
		return false;
	convexGeometry->move(lastMVMatrix);
	if(configSelection->selectionType ==
			ConvexGeometryIntersectionConfigSelection::NEW_SELECTION &&
			configSelection->elementsType != sel->getSelectionType()){
		sel->clearSelectedElements();
		sel->setSelectionType(configSelection->elementsType);
	}else{
		if(sel->getSelectedElements().empty())
			sel->setSelectionType(configSelection->elementsType);
		if(sel->getSelectionType()!= configSelection->elementsType)
			return false;
	}
	return true;
}

bool ConvexGeometryIntersectionRenderer::isFullFilled( vis::Polygon * ){
	return configSelection->elementsType == vis::CONSTANTS::POLYGON;
}

bool ConvexGeometryIntersectionRenderer::isFullFilled( vis::Polyhedron *) {
	return configSelection->elementsType == vis::CONSTANTS::POLYHEDRON;
}

bool ConvexGeometryIntersectionRenderer::isFullFilled( vis::Vertex *) {
	return configSelection->selectionType == vis::CONSTANTS::VERTEX;
}

bool ConvexGeometryIntersectionRenderer::setup(){
	configSelection->readConfig();
	return true;
}

#include "Factories/RendererRegistry.h"
#include "Factories/selectionstrategyregistry.h"
ConvexGeometryIntersectionRenderer* convexStrategyToRegister =
		new ConvexGeometryIntersectionRenderer();
REGISTER_SELECTION_STRATEGY_INSTANCE(convexStrategyToRegister);
REGISTER_RENDERER_INSTANCE(convexStrategyToRegister);
