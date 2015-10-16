#include "mainrenderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Utils/shaderutils.h"
#include "Utils/matrixtransformationutils.h"
#include "Utils/framebufferobjecthandle.h"
#include "Utils/openglutils.h"
#include <glm/gtc/type_ptr.hpp>

#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS_ATTRIBUTE 1
MainRenderer::MainRenderer():Renderer("Flat colors, wireframe"){

	config = (MainRendererConfig*)0;
	fboHandle = 0;
	depthStencilTextureHandle = 0;
	depthStencilTextureHandle2 = 0;
	finalResultTexture = 0;
	currentLayerTexture = 0;
	depthPeelingProgram = 0;
	rendererWeight = RENDERER_WEIGHT_BASE+0.1f;
}
MainRenderer::~MainRenderer(){
	if(config)
		delete config;
	if(fboHandle)
		delete fboHandle;
	OpenGLUtils::deleteTexture(finalResultTexture);
	OpenGLUtils::deleteTexture(currentLayerTexture);
	OpenGLUtils::deleteTexture(depthStencilTextureHandle);
	OpenGLUtils::deleteTexture(depthStencilTextureHandle2);
}
void MainRenderer::glewIsReadyRenderer(){
	fboSize = glm::vec2(100,100);
	finalResultTexture = OpenGLUtils::uploadRaw2DTexture(0,fboSize.x,fboSize.y,
														 GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
	TextureBufferData texdataFbo = {finalResultTexture,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D};
	fboHandle = new FrameBufferObjectHandle(fboSize.x,fboSize.y,texdataFbo);
	config = new MainRendererConfig();
	if(!fboHandle->isOk())
		config->depthPeelingNotAvailable();
	else{
		depthStencilTextureHandle = fboHandle->getDepthStencilTextureHandle();
		depthStencilTextureHandle2 = OpenGLUtils::uploadRaw2DTexture(0,fboSize.x,fboSize.y,
																	 GL_DEPTH24_STENCIL8,
																	 GL_DEPTH_STENCIL,
																	 GL_UNSIGNED_INT_24_8);

		finalResultTexture = OpenGLUtils::uploadRaw2DTexture(0,fboSize.x,fboSize.y,
															 GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
	}
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/ModelMainRenderer/mmr.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/ModelMainRenderer/mmr.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/ModelMainRenderer/mmr.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData selectAttr = {VERTEX_FLAGS_ATTRIBUTE, "flags"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(selectAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	//Depth peeling program
	ShaderLoadingData fragmentShaderDataDepthPeeling(GL_FRAGMENT_SHADER);
	fragmentShaderDataDepthPeeling.addFile("Rendering/Renderers/ModelMainRenderer/mmrdepthpeeling.frag");
	shaderList.pop_back();
	shaderList.push_back(fragmentShaderDataDepthPeeling);
	depthPeelingProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(depthPeelingProgram == ShaderUtils::FAIL_CREATING_PROGRAM){
		depthPeelingProgram = 0;
		config->depthPeelingNotAvailable();
	}
	//Simple Texture Shader
	ShaderLoadingData vertexShaderDataTex(GL_VERTEX_SHADER);
	vertexShaderDataTex.addFile("Rendering/Renderers/ModelMainRenderer/simpletexture.vert");
	ShaderLoadingData fragmentShaderDataTex(GL_FRAGMENT_SHADER);
	fragmentShaderDataTex.addFile("Rendering/Renderers/ModelMainRenderer/simpletexture.frag");

	std::vector<ShaderLoadingData> shaderListTex;
	shaderListTex.push_back(vertexShaderDataTex);
	shaderListTex.push_back(fragmentShaderDataTex);

	VertexAttributeBindingData positionAttrTex = {POSITION_ATTRIBUTE, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeListTex;
	attributeListTex.push_back(positionAttrTex);

	textureShaderProgram = ShaderUtils::CreateProgram(shaderListTex,attributeListTex);
	if(textureShaderProgram == ShaderUtils::FAIL_CREATING_PROGRAM){
		config->depthPeelingNotAvailable();
		textureShaderProgram = 0;
	}
	this->applyConfigChanges();
}

bool MainRenderer::resizeFBO(RModel* rmodel){
	if(!fboHandle || !rmodel)
		return false;
	glm::vec2 newSize = rmodel->getViewPortSize();
	if(newSize.x == fboSize.x && newSize.y == fboSize.y)
		return true;
	fboSize = newSize;
	fboHandle->setNewSize(fboSize.x,fboSize.y);
	OpenGLUtils::deleteTexture(finalResultTexture);
	OpenGLUtils::deleteTexture(currentLayerTexture);
	OpenGLUtils::deleteTexture(depthStencilTextureHandle);
	OpenGLUtils::deleteTexture(depthStencilTextureHandle2);
	currentLayerTexture = OpenGLUtils::uploadRaw2DTexture(0,fboSize.x,fboSize.y,
														  GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
	finalResultTexture = OpenGLUtils::uploadRaw2DTexture(0,fboSize.x,fboSize.y,
														 GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
	depthStencilTextureHandle = OpenGLUtils::uploadRaw2DTexture(0,fboSize.x,fboSize.y,
																GL_DEPTH24_STENCIL8,
																GL_DEPTH_STENCIL,
																GL_UNSIGNED_INT_24_8);
	depthStencilTextureHandle2 = OpenGLUtils::uploadRaw2DTexture(0,fboSize.x,fboSize.y,
																 GL_DEPTH24_STENCIL8,
																 GL_DEPTH_STENCIL,
																 GL_UNSIGNED_INT_24_8);
	return true;
}

void MainRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	if(config->useDepthPeeling){
		drawWithDepthPeeling(rmodel);
		return;
	}
	glUseProgram(theProgram);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(theProgram, "ViewportMatrix",rmodel->getViewPortMatrix());
	ShaderUtils::setUniform(theProgram, "Line.Width",config->wireFrameLineWidthM);
	ShaderUtils::setUniform(theProgram, "Line.Color",config->wireFrameColors);
	ShaderUtils::setUniform(theProgram, "Line.isDrawn",config->drawWireFrame);
	ShaderUtils::setUniform(theProgram, "ModelBaseColor",config->baseModelColors);
	ShaderUtils::setUniform(theProgram, "ElementDrawOption",config->elementDrawnOption);
	ShaderUtils::setUniform(theProgram, "SelectedElementColor",config->selectedElementColors);
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the flag buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

}
void MainRenderer::drawWithDepthPeeling(RModel* rmodel){
	if(!resizeFBO(rmodel) || !depthPeelingProgram)
		return;
	fboHandle->bind();
	glEnable(GL_TEXTURE_2D);
	//clear final result texture
	fboHandle->attachTexture(TextureBufferData(finalResultTexture,
											   GL_COLOR_ATTACHMENT0,
											   GL_TEXTURE_2D),true);
	fboHandle->attachTexture(TextureBufferData(depthStencilTextureHandle,
											   GL_DEPTH_STENCIL_ATTACHMENT,
											   GL_TEXTURE_2D),true);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendEquation( GL_FUNC_ADD );
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	for(int i = 0;i<config->depthPeelingLayers;i++){
		//peeling layers
		fboHandle->attachTexture(TextureBufferData(depthStencilTextureHandle,
												   GL_DEPTH_STENCIL_ATTACHMENT,
												   GL_TEXTURE_2D),true);
		fboHandle->attachTexture(TextureBufferData(currentLayerTexture,
												   GL_COLOR_ATTACHMENT0,
												   GL_TEXTURE_2D),true);
		glUseProgram(depthPeelingProgram);
		ShaderUtils::setUniform(depthPeelingProgram, "MVP",rmodel->getMVP());
		ShaderUtils::setUniform(depthPeelingProgram, "ViewportSize",fboSize);
		ShaderUtils::setUniform(depthPeelingProgram, "ViewportMatrix",rmodel->getViewPortMatrix());
		ShaderUtils::setUniform(depthPeelingProgram, "Line.Width",config->wireFrameLineWidthM);
		ShaderUtils::setUniform(depthPeelingProgram, "Line.Color",config->wireFrameColors);
		ShaderUtils::setUniform(depthPeelingProgram, "Line.isDrawn",config->drawWireFrame);
		ShaderUtils::setUniform(depthPeelingProgram, "ModelBaseColor",config->baseModelColors);
		ShaderUtils::setUniform(depthPeelingProgram, "ElementDrawOption",config->elementDrawnOption);
		ShaderUtils::setUniform(depthPeelingProgram, "SelectedElementColor",config->selectedElementColors);
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
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE, GL_ZERO);
		glDepthFunc(GL_LESS);
		glDepthMask(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ShaderUtils::setUniformTexture(depthPeelingProgram,"TexColorsDepth",0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthStencilTextureHandle2);
		glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_DEPTH_COMPONENT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		ShaderUtils::setUniform(depthPeelingProgram, "stageNumber",i);
		glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
		glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
		glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
		glBindBuffer(GL_ARRAY_BUFFER,0);
		//layer peeled, preparing to add it to the final result texture
		fboHandle->attachTexture(TextureBufferData(finalResultTexture,
												   GL_COLOR_ATTACHMENT0,
												   GL_TEXTURE_2D),true);
		fboHandle->attachTexture(TextureBufferData(depthStencilTextureHandle2,
												   GL_DEPTH_STENCIL_ATTACHMENT,
												   GL_TEXTURE_2D),true);
		//aggregate new layer to final image texture
		glUseProgram(textureShaderProgram);
		ShaderUtils::setUniform(textureShaderProgram,"finalStage",0);
		glDisable(GL_DEPTH_TEST);//no depth test required
		//set blend function to blend from front to back
		//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);//later layers after the current
		glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE,GL_ZERO,GL_ONE_MINUS_SRC_ALPHA);
		ShaderUtils::setUniformTexture(textureShaderProgram,"textureObj",0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, currentLayerTexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		float tripos[12] = {-1,-1,1,-1,-1,1,
							1,-1,1,1,-1,1};

		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer(2,GL_FLOAT,0,&tripos[0]);

		glDrawArrays(GL_TRIANGLES,0,12);

		glDisableClientState(GL_VERTEX_ARRAY );

		glUseProgram(0);
		/*char ss[] = {'0','1','2','3','4','5'}; //textures to files
		std::string sscom("texyvueNCurrentLayer.raw");
		std::string sffscom("texyvueNFinalResult.raw");
		std::string ssDecom("texyvueNDepth.raw");
		std::string ssDecom2("texyvueNDepth2.raw");
		sscom[7] = ss[i];
		ssDecom[7] = ss[i];
		ssDecom2[7] = ss[i];
		sffscom[7] = ss[i];
		OpenGLUtils::getColorTextureToFile(sscom,fboSize.x,fboSize.y,
										   4,currentLayerTexture,GL_RGBA,GL_UNSIGNED_BYTE);
		OpenGLUtils::getColorTextureToFile(sffscom,fboSize.x,fboSize.y,
										   4,finalResultTexture,GL_RGBA,GL_UNSIGNED_BYTE);
		OpenGLUtils::getDepthStencilTextureToFile(ssDecom2,fboSize.x,fboSize.y,
										   depthStencilTextureHandle2);
		OpenGLUtils::getDepthStencilTextureToFile(ssDecom,fboSize.x,fboSize.y,
										   depthStencilTextureHandle);//*/
		//swap depth buffers
		GLuint temp = depthStencilTextureHandle;
		depthStencilTextureHandle = depthStencilTextureHandle2;
		depthStencilTextureHandle2 = temp;
	}
	fboHandle->attachTexture(TextureBufferData(0,
											   GL_COLOR_ATTACHMENT0,
											   GL_TEXTURE_2D),true);
	fboHandle->attachTexture(TextureBufferData(0,
											   GL_DEPTH_STENCIL_ATTACHMENT,
											   GL_TEXTURE_2D),true);
	fboHandle->unbind();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	glBlendEquation( GL_FUNC_ADD );
	//glBlendFunc(GL_ONE, GL_SRC_ALPHA);//Mix resulting image with background
	glBlendFunc(GL_ONE, GL_ZERO);
	glUseProgram(textureShaderProgram);
	ShaderUtils::setUniform(textureShaderProgram,"backgroundColor",rmodel->getBackgroundColor());
	ShaderUtils::setUniform(textureShaderProgram,"finalStage",1);
	ShaderUtils::setUniformTexture(textureShaderProgram,"textureObj",0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalResultTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float tripos[12] = {-1,-1,1,-1,-1,1,
						1,-1,1,1,-1,1};

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer(2,GL_FLOAT,0,&tripos[0]);

	glDrawArrays(GL_TRIANGLES,0,12);

	glDisableClientState(GL_VERTEX_ARRAY );

	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	//draw finally to screen buffer
}

BaseRendererConfig* MainRenderer::getRendererConfigWidget(){
	return this->config;
}

bool MainRenderer::hasRendererConfigWidget(){
	return true;
}
void MainRenderer::applyConfigChanges(RModel *){
	this->config->readConfig();
}
#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(MainRenderer);
