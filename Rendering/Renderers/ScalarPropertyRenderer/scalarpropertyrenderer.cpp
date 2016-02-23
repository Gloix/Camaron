#include "scalarpropertyrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Utils/shaderutils.h"
#include "Utils/openglutils.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Common/Constants.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"

#define POSITION_ATTRIBUTE 0
#define VERTEX_SCALARPROP 1

ScalarPropertyRenderer::ScalarPropertyRenderer():Renderer("Scalar Property Coloring"){

	config = (ScalarPropertyRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+2.0f;
}
ScalarPropertyRenderer::~ScalarPropertyRenderer(){
	if(config)
		delete config;

}
void ScalarPropertyRenderer::glewIsReadyRenderer(){
	//load shaders and initial values
	config = new ScalarPropertyRendererConfig();
	config->readConfig();

	ShaderLoadingData vertexShaderData1(GL_VERTEX_SHADER);
	vertexShaderData1.addFile("Rendering/Renderers/ScalarPropertyRenderer/spr.vert");
	ShaderLoadingData fragmentShaderData1(GL_FRAGMENT_SHADER);
	fragmentShaderData1.addFile("Rendering/Renderers/ScalarPropertyRenderer/spr.frag");

	std::vector<ShaderLoadingData> shaderList1;
	shaderList1.push_back(vertexShaderData1);
	shaderList1.push_back(fragmentShaderData1);

	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "in_Position"};
	VertexAttributeBindingData scalarPropAttr = {VERTEX_SCALARPROP, "in_ScalarProp"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(scalarPropAttr);

	program = ShaderUtils::CreateProgram(shaderList1,attributeList);
	if(program == ShaderUtils::FAIL_CREATING_PROGRAM){
		this->ok = false;
		return;
	}

}

void ScalarPropertyRenderer::draw(RModel* rmodel){
	if(!rmodel || !config->selectedScalarDef)
		return;

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	// FIRST DRAW
	glUseProgram(program);
	ShaderUtils::setUniform(program,"u_TransformMatrix",rmodel->getMVP());
	ShaderUtils::setUniform(program,"coloring_type",config->coloring_type);
	ShaderUtils::setUniform(program,"inverse_intensity",config->inverse_intensity);
	ShaderUtils::setUniform(program,"min_bound",config->selectedBounds[(PropertyFieldDef*)config->selectedScalarDef.get()][0]);
	ShaderUtils::setUniform(program,"max_bound",config->selectedBounds[(PropertyFieldDef*)config->selectedScalarDef.get()][1]);

	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );

	auto currentRModelPropertyFieldDef = rmodel->loadPropertyField(config->selectedScalarDef);

	// Map index 1 to the vertex scalar buffer
	glBindBuffer(GL_ARRAY_BUFFER, currentRModelPropertyFieldDef->getBuffer());
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, currentRModelPropertyFieldDef->getStride(),
						   currentRModelPropertyFieldDef->getOffset());
	if(rmodel->getModelType()== 0)//vis::CONSTANTS::VERTEX_CLOUD)
		glDrawArrays(GL_POINTS, 0, rmodel->vertexFlagsAttribute.size() );
	else
		glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);
}

BaseRendererConfig* ScalarPropertyRenderer::getRendererConfigWidget(){
	return this->config;
}

bool ScalarPropertyRenderer::hasRendererConfigWidget(){
	return true;
}
void ScalarPropertyRenderer::applyConfigChanges(RModel *){
	this->config->readConfig();
}

bool ScalarPropertyRenderer::rmodelChanged(RModel* rmodel){
	if(rmodel->bounds.size()<6 || !rmodel->getOriginalModel() ||
			rmodel->getModelType()==vis::CONSTANTS::VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH ||
			rmodel->getModelType()==vis::CONSTANTS::LIGHT_WEIGHT_VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::NO_MODEL)
		return false;

	lastModel = rmodel->getOriginalModel();
	std::vector<std::shared_ptr<ScalarFieldDef>> scalarFields;
	ScalarFieldListAdderVisitor visitor(scalarFields);
	for(std::shared_ptr<PropertyFieldDef> prop : lastModel->getPropertyFieldDefs()) {
		prop->accept(visitor);
	}
	if(scalarFields.size() == 0)
		return false;
	config->setScalarFields(scalarFields);

	return true;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(ScalarPropertyRenderer);
