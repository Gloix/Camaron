#include "depthrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Rendering/Renderers/DepthRenderer/depthrendererconfig.h"
#include "Utils/framebufferobjecthandle.h"
#include "Utils/shaderutils.h"
#include "Common/Constants.h"
#define POSITION_ATTRIBUTE 0

DepthRenderer::DepthRenderer():
	Renderer("Depth Renderer"),
	viewportSize(0.0f,0.0f)
{
	config = 0;
	rendererWeight = RENDERER_WEIGHT_BASE+6.8f;
	fboHandle = 0;
}

DepthRenderer::~DepthRenderer(){
	if(config)
		delete config;
}
void DepthRenderer::glewIsReadyRenderer(){
	config = new DepthRendererConfig();
	ShaderLoadingData vertexShaderData1(GL_VERTEX_SHADER);
	vertexShaderData1.addFile("Rendering/Renderers/DepthRenderer/drbs.vert");
	ShaderLoadingData fragmentShaderData1(GL_FRAGMENT_SHADER);
	fragmentShaderData1.addFile("Rendering/Renderers/DepthRenderer/drbs.frag");

	std::vector<ShaderLoadingData> shaderList1;
	shaderList1.push_back(vertexShaderData1);
	shaderList1.push_back(fragmentShaderData1);


	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);

	program1 = ShaderUtils::CreateProgram(shaderList1,attributeList);
	if(program1 == ShaderUtils::FAIL_CREATING_PROGRAM){
		this->ok = false;
		return;
	}
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/DepthRenderer/drv.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/DepthRenderer/drf.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);


	program2 = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(program2 == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	config->readConfig();
}
void DepthRenderer::draw(RModel* rmodel){
	if(!rmodel)
		return;
	if(!resize(rmodel->getViewPortSize()))
		return;
	fboHandle->bind();
	fboHandle->attachTexture(TextureBufferData(depthTextureHandle,
											   GL_DEPTH_STENCIL_ATTACHMENT,
											   GL_TEXTURE_2D),true);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(config->depthFunc);
	glClear(GL_DEPTH_BUFFER_BIT);
	// FIRST DRAW
	glUseProgram(program1);
	ShaderUtils::setUniform(program1, "MVP",rmodel->getMVP());

	glEnableVertexAttribArray(POSITION_ATTRIBUTE);
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	if(rmodel->getModelType()==vis::CONSTANTS::VERTEX_CLOUD)
		glDrawArrays(GL_POINTS, 0, rmodel->vertexFlagsAttribute.size() );
	else
		glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

	fboHandle->attachTexture(TextureBufferData(0,GL_DEPTH_STENCIL_ATTACHMENT,
											   GL_TEXTURE_2D),true);
	fboHandle->unbind();

	//Draw Second stage
	glUseProgram(program2);
	glEnable(GL_TEXTURE_2D);
	ShaderUtils::setUniform(program2,"unicolor",config->modelBaseColor);
	ShaderUtils::setUniformTexture(program2,"TexColorsDepth",0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTextureHandle);
	glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_DEPTH_COMPONENT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float tripos[12] = {-1,-1,1,-1,-1,1,
					   1,-1,1,1,-1,1};

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer(2,GL_FLOAT,0,&tripos[0]);

	glDrawArrays(GL_TRIANGLES,0,12);

	glDisableClientState(GL_VERTEX_ARRAY );
	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);

}
void DepthRenderer::applyConfigChanges(RModel *){
	config->readConfig();
}
BaseRendererConfig* DepthRenderer::getRendererConfigWidget(){
	return config;
}

bool DepthRenderer::hasRendererConfigWidget(){
	return true;
}

bool DepthRenderer::resize(glm::vec2 newSize){
	if(newSize.x == viewportSize.x && newSize.y == viewportSize.y)
		return fboHandle!=0;
	viewportSize = newSize;
	if(fboHandle)
		delete fboHandle;
	if(depthTextureHandle)
		glDeleteTextures(1,&depthTextureHandle);
	fboHandle = new FrameBufferObjectHandle(viewportSize.x,viewportSize.y,true);
	if(!fboHandle->isOk()){
		delete fboHandle;
		return false;
	}
	depthTextureHandle = fboHandle->getDepthStencilTextureHandle();
	return true;
}
bool DepthRenderer::rmodelChanged(RModel* rmodel){
	if(!rmodel || rmodel->getModelType() == vis::CONSTANTS::NO_MODEL)
		return false;
	return true;
}


#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(DepthRenderer);
