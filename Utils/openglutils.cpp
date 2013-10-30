#include "openglutils.h"
#include <iostream>
#include <fstream>
#include "Utils/fileutils.h"
#include "Common/Constants.h"
#include <QFileInfo>
OpenGLUtils* OpenGLUtils::singletonInstance = (OpenGLUtils*)0;

OpenGLUtils::OpenGLUtils()
{
	TextureCount = 0;
}
OpenGLUtils::~OpenGLUtils(){

	std::cout<<"[OpenGLUtils - Destroyer]Textures Count: "<<OpenGLUtils::TextureCount << ", deleting..."<<std::endl;
	std::map<GLuint, int>::iterator iter;
	for (iter = activeTextures.begin(); iter != activeTextures.end(); ++iter){
		GLuint textureHandle = iter->first;
		glDeleteTextures(1,&textureHandle);
	}
}

bool OpenGLUtils::printOpenGLError(){
	GLenum errorCode = glGetError();
	if(errorCode!=GL_NO_ERROR){
		std::cout << OpenGLUtils::errorToString(errorCode) << std::endl;
		return false;//ERRORS
	}
	return true;
}
std::string OpenGLUtils::errorToString(GLenum error){
	switch(error){
		case GL_NO_ERROR:{
			return "No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
		}

		case GL_INVALID_ENUM:{
			return "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
		}

		case GL_INVALID_VALUE:{
			return "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
		}

		case GL_INVALID_OPERATION:{
			return "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
		}

		case GL_STACK_OVERFLOW:{
			return "This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
		}

		case GL_STACK_UNDERFLOW:{
			return "This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
		}

		case GL_OUT_OF_MEMORY:{
			return "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		}

		case GL_TABLE_TOO_LARGE:{
			return "The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.:";
		}
		default:{
			return "Unknown Error.";
		}
	}
}
bool OpenGLUtils::checkAndPrintFramebufferStatus()
{
	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE:
			std::cout << "Framebuffer complete." << std::endl;
			return true;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
			return false;
			/*
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
		std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
		std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
		return false;
*/
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
			return false;

		default:
			std::cout << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
			return false;
	}
}
GLuint OpenGLUtils::uploadRaw2DTexture(std::string filename,unsigned int width,
									   unsigned int height,
									   GLuint internalFormat, GLenum format,GLenum type){
	QFile qfile(QString::fromStdString(IMAGES_RESOURCES+filename));
	if(!qfile.open(QFile::ReadOnly)){
		std::cerr<< "Texture resource not found: "<< (IMAGES_RESOURCES+filename) <<std::endl;
		return 0u;
	}
	int len = qfile.size();
	unsigned char *textureData = new unsigned char[len];
	qfile.read((char*)textureData, len);
	qfile.close();
	if(!textureData)
		return 0u;
	GLuint tid = uploadRaw2DTexture(textureData,
									width,height,
									internalFormat,
									format,type);
	delete[] textureData;
	return tid;

}

GLuint OpenGLUtils::uploadRaw2DTexture(unsigned char* data,unsigned int width,
									   unsigned int height,
									   GLuint internalFormat, GLenum format,GLenum type){

	GLuint tid;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width,
				 height, 0, format, type,
				 data);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	registerNewTexture(tid);
	return tid;
}

GLuint OpenGLUtils::uploadRaw1DTexture(unsigned char* data,unsigned int width,
									   GLuint internalFormat, GLenum format,GLenum type){
	GLuint tid;
	glEnable(GL_TEXTURE_1D);
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_1D, tid);
	glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width,
				 0, format, type,
				 data);
	glBindTexture(GL_TEXTURE_1D, 0);
	glDisable(GL_TEXTURE_1D);
	registerNewTexture(tid);
	return tid;
}
void OpenGLUtils::bindFrameBufferObject(GLuint handle){

}
GLuint OpenGLUtils::getCurrentFrameBufferHandle(){
	GLint handle;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING,&handle);
	return handle;
}
void OpenGLUtils::getColorTextureToFile(std::string filename,int width,int height, int depth, GLuint texturehandle,
										GLenum format, GLenum type){
	char* texContent = new char[width*height*depth];
	glEnable(GL_TEXTURE_2D);
	std::cout << "Texture "<<filename<<", size: (" << width<<", "<<height<<")"<<std::endl;
	glBindTexture(GL_TEXTURE_2D,texturehandle);
	glGetTexImage(GL_TEXTURE_2D,0,format,type,texContent);
	//glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,texContent);
	std::fstream f(filename.c_str(),std::fstream::out | std::fstream::binary |
				   std::fstream::trunc);
	f.write(texContent,width*height*depth);
	f.close();
	delete[] texContent;
}
void OpenGLUtils::getDepthStencilTextureToFile(std::string filename,
											   int width,int height,
											   GLuint texturehandle,
											   bool stencilValue){
	int sizet = width*height;
	char* texContent = new char[sizet*4];
	char* texContentByte = new char[width*height];
	glEnable(GL_TEXTURE_2D);
	std::cout << "Texture DepthStencil "<<filename<<", size: (" << width<<", "<<height<<")"<<std::endl;
	glBindTexture(GL_TEXTURE_2D,texturehandle);
	if(stencilValue)
		glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_STENCIL_INDEX);
	else
		glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_DEPTH_COMPONENT);
	glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,texContent);
	//glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_STENCIL,GL_UNSIGNED_BYTE,texContent);
	for(int i = 0;i<sizet;++i)
		texContentByte[i] = texContent[i*4+3];
	if(OpenGLUtils::printOpenGLError())
		std::cout << "NO HAY ERROR AQUI222"<<std::endl;
	std::fstream f(filename.c_str(),std::fstream::out | std::fstream::binary |
				   std::fstream::trunc);
	f.write(texContentByte,sizet);
	f.close();
	delete[] texContent;
	delete[] texContentByte;

}

void OpenGLUtils::deleteTexture(GLuint texturehandle){
	if(!texturehandle)
		return;
	OpenGLUtils* instance = OpenGLUtils::getSingletonInstance();
	if(!instance)
		return;

	std::cout << "Deleting texture "<< texturehandle << std::endl;
	if(!instance->activeTextures.count(texturehandle)){
		std::cerr << "Trying to delete unregistered texture "<<texturehandle<<std::endl;
		glDeleteTextures(1,&texturehandle);//just in case it needed to be deleted but it wasnt registered (shouldnt happen)
		return;//discard action if it already exist, shouldnt happen
	}
	glDeleteTextures(1,&texturehandle);
	instance->TextureCount--;
	instance->activeTextures.erase(texturehandle);
}
OpenGLUtils* OpenGLUtils::getSingletonInstance(){
	if(!singletonInstance)
		singletonInstance = new OpenGLUtils();
	return singletonInstance;
}
void OpenGLUtils::deleteSingletonInstance(){
	if(singletonInstance)
		delete singletonInstance;
	singletonInstance = 0;
}

void OpenGLUtils::registerNewTexture(GLuint tex){
	if(!tex)
		return;
	std::cout << "Registering texture "<< tex << std::endl;
	OpenGLUtils* instance = OpenGLUtils::getSingletonInstance();
	if(!instance)
		return;
	if(instance->activeTextures.count(tex)){
		std::cerr << "Trying to register texture "<<tex<<" more than once."<<std::endl;
		return;//discard action if it already exist, shouldnt happen
	}
	instance->activeTextures.insert(std::make_pair( tex, 1 ));
	instance->TextureCount++;
}
