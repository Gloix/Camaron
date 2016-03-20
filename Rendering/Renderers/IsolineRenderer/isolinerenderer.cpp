#include <algorithm>
#include "isolinerenderer.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"
#include "Common/Constants.h"
#include "Utils/crosstimer.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS 1
#define VERTEX_SCALARPROP 2
#define VERTEX_FLAGS_ATTRIBUTE 3
IsolineRenderer::IsolineRenderer():
	Renderer("Isolines")
{
	config = (StepsRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+1.0f;
}

IsolineRenderer::~IsolineRenderer(){
	if(config)
		delete config;
}
void IsolineRenderer::glewIsReadyRenderer(){
	config = new StepsRendererConfig();
	this->ok = this->buildIsolineRenderProgram() && this->buildIsolineGenerationProgram();
	if(this->ok) {
		glGenTransformFeedbacks(1, &transformFeedback);
		glGenBuffers(1, &isolinesBuffer);
	}
}

bool IsolineRenderer::buildIsolineRenderProgram() {
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);

	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData scalarAttr = {VERTEX_SCALARPROP, "VertexScalar"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(scalarAttr);

	renderProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	return renderProgram != ShaderUtils::FAIL_CREATING_PROGRAM;

}

bool IsolineRenderer::buildIsolineGenerationProgram() {
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir_gen.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir_gen.geom");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);

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

void IsolineRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object

	if(rModelChanged
			|| config->selectedScalarDef != lastSelectedScalarDef
			|| config->isolevels != lastConfigScalarLevels
			|| lastConfigElementDrawnOption != config->elementDrawnOption) {
		std::vector<float> isolevelSteps = config->isolevels;
		if(isolevelSteps.size() != 0) {
			if(rModelChanged || lastConfigScalarLevels.size()!=config->isolevels.size()) {
				glBindBuffer(GL_ARRAY_BUFFER, isolinesBuffer);
				glBufferData(GL_ARRAY_BUFFER, rmodel->numberOfTriangles*2*16*isolevelSteps.size(), NULL, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				rModelChanged = false;
			}
			glFinish();
			CrossTimer timer;
			generateIsolines(rmodel, isolevelSteps);
			glFinish();
			std::cout << "ISOLINES GENERATION: " << isolevelSteps[0] << "\t" <<
					  timer.getTranscurredNanoseconds() << std::endl;
		} else {
			return;
		}
		lastConfigElementDrawnOption = config->elementDrawnOption;
		lastConfigScalarLevels = config->isolevels;
		lastSelectedScalarDef = config->selectedScalarDef;
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

	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
	//glEnableVertexAttribArray(VERTEX_FLAGS); // Vertex flags
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, isolinesBuffer);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 16,
						   (const GLvoid *)NULL );
	// Map index 1 to the scalar buffer
	glBindBuffer(GL_ARRAY_BUFFER, isolinesBuffer);
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, 16,
						   (const GLvoid *)12 );
	// Map index 2 to the flags buffer
	//glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	//glVertexAttribIPointer( VERTEX_FLAGS, 1, GL_UNSIGNED_INT, 0,
	//						(GLubyte *)NULL );
	glDrawTransformFeedback(GL_LINES, transformFeedback);
	//glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex selection
	//glDisableVertexAttribArray(VERTEX_FLAGS); // Vertex selection

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

}

void IsolineRenderer::generateIsolines(RModel* rmodel, std::vector<float>& values){
	glUseProgram(generateProgram);

	ShaderUtils::setUniform(generateProgram, "Isolevels", values);
	ShaderUtils::setUniform(generateProgram, "IsolevelsSize", (int)values.size());
	ShaderUtils::setUniform(generateProgram, "ElementDrawOption",config->elementDrawnOption);

	// We don't want to rasterize anything. We just want to generate the isosurfaces.
	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, isolinesBuffer);
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex flags

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );

	auto currentRModelPropertyFieldDef = rmodel->loadPropertyField(config->selectedScalarDef);

	glBindBuffer(GL_ARRAY_BUFFER, currentRModelPropertyFieldDef->getBuffer());
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, currentRModelPropertyFieldDef->getStride(),
						   currentRModelPropertyFieldDef->getOffset());

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size());
	glEndTransformFeedback();


	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex selection
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex flags

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

	glDisable(GL_RASTERIZER_DISCARD);

}

BaseRendererConfig* IsolineRenderer::getRendererConfigWidget(){
	return config;
}

bool IsolineRenderer::hasRendererConfigWidget(){
	return true;
}

void IsolineRenderer::applyConfigChanges(RModel *){
	config->readConfig();
}
bool IsolineRenderer::rmodelChanged(RModel* rmodel){
	lastConfigScalarLevels.clear();
	lastSelectedScalarDef = nullptr;
	lastConfigElementDrawnOption = -1;
	rModelChanged = true;
	if(rmodel->bounds.size()<6 || !rmodel->getOriginalModel() ||
			rmodel->getModelType()==vis::CONSTANTS::VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::NO_MODEL)
		return false;

	std::vector<std::shared_ptr<ScalarFieldDef>> scalarFields;
	ScalarFieldListAdderVisitor visitor(scalarFields);
	for(std::shared_ptr<PropertyFieldDef> prop : rmodel->getOriginalModel()->getPropertyFieldDefs()) {
		prop->accept(visitor);
	}
	if(scalarFields.size() == 0) {
		return false;
	}
	config->setScalarFields(scalarFields);
	return true;

}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(IsolineRenderer);
