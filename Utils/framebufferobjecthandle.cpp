#include "framebufferobjecthandle.h"
#include "Utils/openglutils.h"
#include <iostream>
FrameBufferObjectHandle::FrameBufferObjectHandle(int w, int h, ColorRenderBufferData data){
	frameTextureBuffer = 0;
	textureDefaultHandle = 0;
	depthStencilTextureHandle = 0;
	std::vector<ColorRenderBufferData> ren;
	ren.push_back(data);
	initialize(w,h,ren,std::vector<TextureBufferData>());
}

FrameBufferObjectHandle::FrameBufferObjectHandle(int w, int h, TextureBufferData data){
	frameTextureBuffer = 0;
	textureDefaultHandle = 0;
	depthStencilTextureHandle = 0;
	std::vector<TextureBufferData> tex;
	tex.push_back(data);
	initialize(w,h,std::vector<ColorRenderBufferData>(),tex);
}

FrameBufferObjectHandle::FrameBufferObjectHandle(int w, int h, bool textureBuffer){
	frameTextureBuffer = 0;
	textureDefaultHandle = 0;
	depthStencilTextureHandle = 0;
	if(textureBuffer){
		textureDefaultHandle = OpenGLUtils::uploadRaw2DTexture(0,w,h,GL_RGBA32F,GL_RGBA,GL_UNSIGNED_BYTE);
		std::vector<TextureBufferData> tex;
		TextureBufferData data = {textureDefaultHandle,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D};
		tex.push_back(data);
		initialize(w,h,std::vector<ColorRenderBufferData>(),tex);
	}
	else{
		std::vector<ColorRenderBufferData> colorRenderBuffers;
		ColorRenderBufferData data = {GL_RGBA,GL_COLOR_ATTACHMENT0};
		colorRenderBuffers.push_back(data);
		initialize(w,h,colorRenderBuffers,std::vector<TextureBufferData>());
	}
}

FrameBufferObjectHandle::FrameBufferObjectHandle(int w, int h,
												 std::vector<ColorRenderBufferData> colorRenderBuffers)
{
	textureDefaultHandle = 0;
	frameTextureBuffer = 0;
	depthStencilTextureHandle = 0;
	initialize(w,h,colorRenderBuffers,std::vector<TextureBufferData>());

}
FrameBufferObjectHandle::FrameBufferObjectHandle(int w, int h,
												 std::vector<TextureBufferData> bb)
{
	textureDefaultHandle = 0;
	frameTextureBuffer = 0;
	depthStencilTextureHandle = 0;
	initialize(w,h,std::vector<ColorRenderBufferData>(),bb);

}
void FrameBufferObjectHandle::initialize(int w, int h,
										 std::vector<ColorRenderBufferData> colorRenderBuffers,
										 std::vector<TextureBufferData> textureBufferDatas){
	width = w;
	height = h;
	//Depth, Stencil buffer
	depthStencilTextureHandle = OpenGLUtils::uploadRaw2DTexture(0,width,height,
																GL_DEPTH24_STENCIL8,
																GL_DEPTH_STENCIL,
																GL_UNSIGNED_INT_24_8);
	currentFBO = OpenGLUtils::getCurrentFrameBufferHandle();

	// create color buffer object and attached to fbo
	GLuint tempRenderBufferColor;
	for( ColorRenderBufferData colorRenderBuffer : colorRenderBuffers ){
		glGenRenderbuffers(1, &tempRenderBufferColor);
		glBindRenderbuffer(GL_RENDERBUFFER, tempRenderBufferColor);
		glRenderbufferStorage(GL_RENDERBUFFER, colorRenderBuffer.internalformat,
							  width, height);
		renderBufferColors.push_back(tempRenderBufferColor);
		glBindRenderbuffer(GL_RENDERBUFFER, 0); //UnBind
	}

	//Frame buffer
	glGenFramebuffers(1, &frameTextureBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameTextureBuffer);
	// attach a renderbuffer to depth attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
					   GL_TEXTURE_2D, depthStencilTextureHandle, 0);

	for(std::vector<ColorRenderBufferData>::size_type i = 0;i<colorRenderBuffers.size();i++)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, colorRenderBuffers[i].attachment,
								  GL_RENDERBUFFER, renderBufferColors[i]);
	for( TextureBufferData& textureBufferData : textureBufferDatas ){
		if(textureBufferData.textureTarget==GL_TEXTURE_2D)
			glFramebufferTexture2D(GL_FRAMEBUFFER, textureBufferData.attachment,
								   textureBufferData.textureTarget, textureBufferData.textureHandle, 0);
		else if(textureBufferData.textureTarget==GL_TEXTURE_1D)
			continue;
	}
	ok = OpenGLUtils::checkAndPrintFramebufferStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, currentFBO); //unbind the FBO

}

FrameBufferObjectHandle::~FrameBufferObjectHandle(){
	OpenGLUtils::deleteTexture(depthStencilTextureHandle);
	OpenGLUtils::deleteTexture(textureDefaultHandle);
	if(frameTextureBuffer)
		glDeleteFramebuffers(1,&frameTextureBuffer);
	if(renderBufferColors.size()>0u)
		glDeleteRenderbuffers(renderBufferColors.size(),&renderBufferColors[0]);
}

bool FrameBufferObjectHandle::bind(){
	currentFBO = OpenGLUtils::getCurrentFrameBufferHandle();
	glBindFramebuffer(GL_FRAMEBUFFER, frameTextureBuffer);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT);
	glViewport(0,0, width, height);//*/

	/*if(OpenGLUtils::printOpenGLError())
		std::cout << "No errors Binding FBO!" <<std::endl;//*/
	return true;
}

bool FrameBufferObjectHandle::unbind(){
	glPopAttrib();
	if(currentFBO==frameTextureBuffer)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
	/*if(OpenGLUtils::printOpenGLError())
		std::cout << "No errors UnBinding FBO!" <<std::endl;//*/
	return true;
}

bool FrameBufferObjectHandle::isOk(){
	return ok;
}
GLuint FrameBufferObjectHandle::getTextureDefaultHandle(){
	return textureDefaultHandle;
}
bool FrameBufferObjectHandle::attachTexture(TextureBufferData texdata, bool binded){
	if(!ok)
		return false;
	if(!binded)
		glBindFramebuffer(GL_FRAMEBUFFER, frameTextureBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, texdata.attachment,
						   texdata.textureTarget, texdata.textureHandle, 0);

	if(!binded)
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind the FBO
	return true;

}
GLuint FrameBufferObjectHandle::getDepthStencilTextureHandle(){
	return depthStencilTextureHandle;
}
void FrameBufferObjectHandle::setNewSize(int w, int h){
	width = w;
	height = h;
}
