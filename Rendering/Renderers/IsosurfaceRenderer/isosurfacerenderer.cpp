#include <algorithm>
#include "isosurfacerenderer.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Common/Constants.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"
#include "Utils/openglutils.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_SCALARPROP 1
#define VERTEX_FLAGS_ATTRIBUTE 2

char triTable[16][8] = {
	{-1, -1, -1, -1, -1, -1, -1, -1}, //0000
	{ 0,  2,  3, -1, -1, -1, -1, -1},
	{ 0,  1,  4, -1, -1, -1, -1, -1}, //0010
	{ 2,  3,  4,  2,  4,  1, -1, -1},
	{ 2,  5,  1, -1, -1, -1, -1, -1}, //0100
	{ 0,  3,  5,  0,  5,  1, -1, -1},
	{ 0,  4,  5,  0,  5,  2, -1, -1}, //0110
	{ 3,  4,  5, -1, -1, -1, -1, -1},
	{ 3,  4,  5, -1, -1, -1, -1, -1}, //1000
	{ 0,  4,  5,  0,  5,  2, -1, -1},
	{ 0,  3,  5,  0,  5,  1, -1, -1}, //1010
	{ 2,  5,  1, -1, -1, -1, -1, -1},
	{ 2,  3,  4,  2,  4,  1, -1, -1}, //1100
	{ 0,  1,  4, -1, -1, -1, -1, -1},
	{ 0,  2,  3, -1, -1, -1, -1, -1}, //1110
	{-1, -1, -1, -1, -1, -1, -1, -1}};


IsosurfaceRenderer::IsosurfaceRenderer():
	Renderer("Isosurfaces"),
	rModelChanged(true)
{
	config = (StepsRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+1.0f;
	lastModel = NULL;
}

IsosurfaceRenderer::~IsosurfaceRenderer(){
	if(config)
		delete config;
}
void IsosurfaceRenderer::glewIsReadyRenderer(){
	config = new StepsRendererConfig();
	this->ok = this->buildIsosurfaceRenderProgram() && this->buildIsosurfaceGenerationProgram();
	if(this->ok) {
		glGenTransformFeedbacks(1, &transformFeedback);
		glGenBuffers(1, &isosurfacesBuffer);
		
		
		this->triTableTex = OpenGLUtils::uploadRaw2DTexture((unsigned char*)&triTable,8
											   ,16, GL_R32I, GL_RED_INTEGER,GL_BYTE);
		glBindTexture(GL_TEXTURE_2D, this->triTableTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
						GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

bool IsosurfaceRenderer::buildIsosurfaceRenderProgram() {
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/IsosurfaceRenderer/is.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/IsosurfaceRenderer/is.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);


	//VertexAttributeBindingData selectAttr = {VERTEX_NORMAL, "VertexNormal"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	//VertexAttributeBindingData flagAttr = {VERTEX_FLAGS, "VertexFlags"};
	VertexAttributeBindingData scalarAttr = {VERTEX_SCALARPROP, "VertexScalar"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(scalarAttr);
	//attributeList.push_back(flagAttr);

	renderProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	return renderProgram != ShaderUtils::FAIL_CREATING_PROGRAM;

}

bool IsosurfaceRenderer::buildIsosurfaceGenerationProgram() {
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/IsosurfaceRenderer/is_gen.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/IsosurfaceRenderer/is_gen.geom");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);

	//VertexAttributeBindingData selectAttr = {VERTEX_NORMAL, "VertexNormal"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData scalarAttr = {VERTEX_SCALARPROP, "VertexScalar"};
	VertexAttributeBindingData flagsAttr = {VERTEX_FLAGS_ATTRIBUTE , "VertexFlags"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(scalarAttr);
	attributeList.push_back(flagsAttr);

	TransformFeedbackData tfData = {{"vertexPosition", "scalarValue"},GL_INTERLEAVED_ATTRIBS};

	generateProgram = ShaderUtils::CreateProgram(shaderList,attributeList,tfData);
	return generateProgram != ShaderUtils::FAIL_CREATING_PROGRAM;
}

void IsosurfaceRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object

	if(rModelChanged || config->isolevels != lastConfigScalarLevels
			|| lastConfigElementDrawnOption != config->elementDrawnOption) {
		if(rModelChanged) {
			lastModel = rmodel->getOriginalModel();
			glBindBuffer(GL_ARRAY_BUFFER, isosurfacesBuffer);
			glBufferData(GL_ARRAY_BUFFER, rmodel->numberOfTetrahedrons*2*3*16, NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			std::vector<std::shared_ptr<ScalarFieldDef>> scalarFields;
			ScalarFieldListAdderVisitor visitor(scalarFields);
			for(std::shared_ptr<PropertyFieldDef> prop : lastModel->getPropertyFieldDefs()) {
				prop->accept(visitor);
			}
			config->setScalarFields(scalarFields);
			rModelChanged = false;
		}
		lastConfigElementDrawnOption = config->elementDrawnOption;
		lastConfigScalarLevels = config->isolevels;
		std::vector<float> isosurfaceSteps = config->isolevels;
		if(isosurfaceSteps.size() != 0) {
			generateIsosurface(rmodel, isosurfaceSteps);
		} else {
			return;
		}
	}
	if(!config->selectedScalarDef || config->isolevels.size() == 0) {
		return;
	}
	glUseProgram(renderProgram);

	ShaderUtils::setUniform(renderProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(renderProgram, "GradientStartColor", config->gradientStartColor);
	ShaderUtils::setUniform(renderProgram, "GradientEndColor", config->gradientEndColor);
	ShaderUtils::setUniform(renderProgram, "ScalarMin", config->selectedScalarDef->getMin());
	ShaderUtils::setUniform(renderProgram, "ScalarMax", config->selectedScalarDef->getMax());
	//ShaderUtils::setUniform(renderProgram, "WireFrameColor", config->wireframeColor);
	//ShaderUtils::setUniform(renderProgram, "WireFrameOption", config->wireFrameOption);

	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
	//glEnableVertexAttribArray(VERTEX_FLAGS); // Vertex flags
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, isosurfacesBuffer);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 16,
						   (const GLvoid *)NULL );
	// Map index 1 to the scalar buffer
	glBindBuffer(GL_ARRAY_BUFFER, isosurfacesBuffer);
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, 16,
						   (const GLvoid *)12 );
	// Map index 2 to the flags buffer
	//glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	//glVertexAttribIPointer( VERTEX_FLAGS, 1, GL_UNSIGNED_INT, 0,
	//						(GLubyte *)NULL );
	glDrawTransformFeedback(GL_TRIANGLES, transformFeedback);
	//glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex selection
	//glDisableVertexAttribArray(VERTEX_FLAGS); // Vertex selection

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

}

void IsosurfaceRenderer::generateIsosurface(RModel* rmodel, std::vector<float>& values){
	glUseProgram(generateProgram);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, triTableTex);
	ShaderUtils::setUniform(generateProgram, "Isolevels", values);
	ShaderUtils::setUniform(generateProgram, "IsolevelsSize", (int)values.size());
	ShaderUtils::setUniformTexture(generateProgram, "triTableTex", GL_TEXTURE0);
	ShaderUtils::setUniform(generateProgram, "ElementDrawOption",config->elementDrawnOption);

	// We don't want to rasterize anything. We just want to generate the isosurfaces.
	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, isosurfacesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rmodel->tetrahedronVertexIdsBufferObject);
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex flags

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );

//	if(!currentRModelPropertyFieldDef
//			|| currentRModelPropertyFieldDef->getPropertyFieldDef() != config->selectedScalarDef) {
//		// Make sure last property is deleted from the GPU
//		// the RModelPropertyFieldDef object will take care of that.
//		currentRModelPropertyFieldDef = nullptr;

//		currentRModelPropertyFieldDef = rmodel->loadPropertyField(config->selectedScalarDef);
//	}
	auto currentRModelPropertyFieldDef = rmodel->loadPropertyField(config->selectedScalarDef);

	glBindBuffer(GL_ARRAY_BUFFER, currentRModelPropertyFieldDef->getBuffer());
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, currentRModelPropertyFieldDef->getStride(),
						   currentRModelPropertyFieldDef->getOffset());

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );

	glBeginTransformFeedback(GL_POINTS);
	glDrawElements(GL_LINES_ADJACENCY, rmodel->numberOfTetrahedrons*4, GL_UNSIGNED_INT,
				   (GLvoid*)0 );

	glEndTransformFeedback();

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex selection
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex flags

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

	glDisable(GL_RASTERIZER_DISCARD);

}

BaseRendererConfig* IsosurfaceRenderer::getRendererConfigWidget(){
	return config;
}

bool IsosurfaceRenderer::hasRendererConfigWidget(){
	return true;
}

void IsosurfaceRenderer::applyConfigChanges(RModel *){
	config->readConfig();
}

bool IsosurfaceRenderer::rmodelChanged(RModel* rmodel){
	rModelChanged = true;
	if(rmodel->bounds.size()<6 || !rmodel->getOriginalModel() ||
			rmodel->getModelType()==vis::CONSTANTS::VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::POLYGON_MESH ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::NO_MODEL)
		return false;
	return true;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(IsosurfaceRenderer);
