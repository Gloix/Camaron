#include "heightrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Utils/shaderutils.h"
#include "Utils/openglutils.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Common/Constants.h"

HeightRenderer::HeightRenderer():Renderer("Height Coloring"){

    config = (HeightRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+2.0f;
}
HeightRenderer::~HeightRenderer(){
	if(config)
		delete config;

}
void HeightRenderer::glewIsReadyRenderer(){
    //load shaders and initial values
    config = new HeightRendererConfig();
    config->readConfig();

    ShaderLoadingData vertexShaderData1(GL_VERTEX_SHADER);
    vertexShaderData1.addFile("Rendering/Renderers/HeightRenderer/height_shader.vert");
    ShaderLoadingData fragmentShaderData1(GL_FRAGMENT_SHADER);
    fragmentShaderData1.addFile("Rendering/Renderers/HeightRenderer/height_shader.frag");

    std::vector<ShaderLoadingData> shaderList1;
    shaderList1.push_back(vertexShaderData1);
    shaderList1.push_back(fragmentShaderData1);

    VertexAttributeBindingData positionAttr = {0, "in_Position"};
    std::vector<VertexAttributeBindingData> attributeList;
    attributeList.push_back(positionAttr);

    program = ShaderUtils::CreateProgram(shaderList1,attributeList);
    if(program == ShaderUtils::FAIL_CREATING_PROGRAM){
        this->ok = false;
        return;
    }

}

void HeightRenderer::draw(RModel* rmodel){
    if(!rmodel)
        return;
    config->setModel(rmodel);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    // FIRST DRAW
    glUseProgram(program);
    ShaderUtils::setUniform(program,"u_TransformMatrix",rmodel->getMVP());
    ShaderUtils::setUniform(program,"axis",config->axis);
    ShaderUtils::setUniform(program,"coloring_type",config->coloring_type);
    ShaderUtils::setUniform(program,"inverse_intensity",config->inverse_intensity);
    ShaderUtils::setUniform(program,"min_x",config->min_x);
    ShaderUtils::setUniform(program,"min_y",config->min_y);
    ShaderUtils::setUniform(program,"min_z",config->min_z);
    ShaderUtils::setUniform(program,"max_x",config->max_x);
    ShaderUtils::setUniform(program,"max_y",config->max_y);
	ShaderUtils::setUniform(program,"max_z",config->max_z);

    glEnableVertexAttribArray(0);
    // Map index 0 to the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
    if(rmodel->getModelType()== 0)//vis::CONSTANTS::VERTEX_CLOUD)
        glDrawArrays(GL_POINTS, 0, rmodel->vertexFlagsAttribute.size() );
    else
        glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
    glDisableVertexAttribArray(0); // Vertex position
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}

BaseRendererConfig* HeightRenderer::getRendererConfigWidget(){
	return this->config;
}

bool HeightRenderer::hasRendererConfigWidget(){
	return true;
}
void HeightRenderer::applyConfigChanges(RModel *){
	this->config->readConfig();
}
#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(HeightRenderer);
