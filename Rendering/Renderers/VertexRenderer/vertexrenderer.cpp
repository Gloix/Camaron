#include "vertexrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Utils/shaderutils.h"
#include "Utils/matrixtransformationutils.h"
#include <glm/gtc/type_ptr.hpp>

#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS_ATTRIBUTE 1
VertexRenderer::VertexRenderer():Renderer("Vertex Renderer"){
	config = (VertexRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+6.0f;
}
VertexRenderer::~VertexRenderer(){
	if(config)
		delete config;
}
void VertexRenderer::glewIsReadyRenderer(){
	config = new VertexRendererConfig();
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/VertexRenderer/vr.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/VertexRenderer/vr.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData selectAttr = {VERTEX_FLAGS_ATTRIBUTE, "VertexFlags"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(selectAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	this->applyConfigChanges();
}
bool VertexRenderer::rmodelChanged(RModel* rmodel){
	return rmodel->getOriginalModel();
}

void VertexRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	glUseProgram(theProgram);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(theProgram, "BaseColor",config->baseVertexColors);
	ShaderUtils::setUniform(theProgram, "SelectedColor",config->selectedVertexColors);
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
	glDrawArrays(GL_POINTS, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);


}
BaseRendererConfig* VertexRenderer::getRendererConfigWidget(){
	return this->config;
}

bool VertexRenderer::hasRendererConfigWidget(){
	return true;
}
void VertexRenderer::applyConfigChanges(RModel *){
	this->config->applyConfig();
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(VertexRenderer);
