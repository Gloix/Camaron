#include <algorithm>
#include "isolinerenderer.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"
#include "Common/Constants.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS 1
#define VERTEX_SCALARPROP 2
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
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
    vertexShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.vert");
    ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
    geometryShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
    shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


	//VertexAttributeBindingData selectAttr = {VERTEX_NORMAL, "VertexNormal"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData flagAttr = {VERTEX_FLAGS, "VertexFlags"};
	VertexAttributeBindingData scalarAttr = {VERTEX_SCALARPROP, "VertexScalar"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(scalarAttr);
	attributeList.push_back(flagAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	sizeRatio = 1.0f;
}

void IsolineRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object

	if(!config->selectedScalarDef || config->isolevels.size() == 0) {
		return;
	}

	glUseProgram(theProgram);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(theProgram, "Isolevels", config->isolevels);
	ShaderUtils::setUniform(theProgram, "IsolevelsSize", (int)config->isolevels.size());
	ShaderUtils::setUniform(theProgram, "GradientStartColor", config->gradientStartColor);
	ShaderUtils::setUniform(theProgram, "GradientEndColor", config->gradientEndColor);
	ShaderUtils::setUniform(theProgram, "ScalarMin", config->selectedScalarDef->getMin());
	ShaderUtils::setUniform(theProgram, "ScalarMax", config->selectedScalarDef->getMax());
	ShaderUtils::setUniform(theProgram, "WireFrameColor", config->wireframeColor);
	ShaderUtils::setUniform(theProgram, "WireFrameOption", config->wireFrameOption);
	ShaderUtils::setUniform(theProgram, "ElementDrawOption",config->elementDrawnOption);

	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
	glEnableVertexAttribArray(VERTEX_FLAGS); // Vertex flags
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );

	//if(!currentRModelPropertyFieldDef
	//		|| currentRModelPropertyFieldDef->getPropertyFieldDef() != config->selectedScalarDef) {
		// Make sure last property is deleted from the GPU
		// the RModelPropertyFieldDef object will take care of that.
	//	currentRModelPropertyFieldDef = nullptr;

	//
	//}
	auto currentRModelPropertyFieldDef = rmodel->loadPropertyField(config->selectedScalarDef);

	// Map index 1 to the scalar buffer
	glBindBuffer(GL_ARRAY_BUFFER, currentRModelPropertyFieldDef->getBuffer());
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, currentRModelPropertyFieldDef->getStride(),
						   currentRModelPropertyFieldDef->getOffset() );
	// Map index 2 to the flags buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex selection
	glDisableVertexAttribArray(VERTEX_FLAGS); // Vertex selection

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

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
