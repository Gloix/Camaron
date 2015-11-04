#include "scalarpropertyrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Utils/shaderutils.h"
#include "Utils/openglutils.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Common/Constants.h"

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
    if(!rmodel)
        return;
    config->setModel(rmodel);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    // FIRST DRAW
    glUseProgram(program);
    ShaderUtils::setUniform(program,"u_TransformMatrix",rmodel->getMVP());
    ShaderUtils::setUniform(program,"coloring_type",config->coloring_type);
    ShaderUtils::setUniform(program,"inverse_intensity",config->inverse_intensity);
	ShaderUtils::setUniform(program,"min_bound",config->selectedBounds[config->selectedScalarDef][0]);
	ShaderUtils::setUniform(program,"max_bound",config->selectedBounds[config->selectedScalarDef][1]);

	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
    // Map index 0 to the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the vertex scalar buffer
	glBindBuffer(GL_ARRAY_BUFFER, config->selectedScalarDef->buffer);
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, config->selectedScalarDef->stride,
						   (GLubyte *)config->selectedScalarDef->offset);
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
#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(ScalarPropertyRenderer);
