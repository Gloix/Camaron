#ifndef OPENGLUTILS_H
#define OPENGLUTILS_H
#include <string>
#include <GL/glew.h>
#include <map>

class OpenGLUtils
{
	public:
		static bool printOpenGLError();
		static bool checkAndPrintFramebufferStatus();
		static GLuint uploadRaw2DTexture(std::string filename,unsigned int width,
										 unsigned int height, GLuint internalFormat,
										 GLenum format, GLenum type);
		static GLuint uploadRaw2DTexture(unsigned char* data,unsigned int width,
										 unsigned int height,GLuint internalFormat,
										 GLenum format,GLenum type);
		static GLuint uploadRaw1DTexture(unsigned char* data,unsigned int width,
										 GLuint internalFormat, GLenum format,
										 GLenum type);
		static void deleteTexture(GLuint texturehandle);

		static void bindFrameBufferObject(GLuint handle);
		static GLuint getCurrentFrameBufferHandle();
		static void getColorTextureToFile(std::string filename,int width,int height,
										  int depth, GLuint texturehandle,
										  GLenum format, GLenum type);
		static void getDepthStencilTextureToFile(std::string filename, int width,
												 int height, GLuint texturehandle,
												 bool stencilValue = false);
		static OpenGLUtils* singletonInstance;
		static void deleteSingletonInstance();
		static OpenGLUtils* getSingletonInstance();
	private:
		OpenGLUtils();
		~OpenGLUtils();
		static void registerNewTexture(GLuint);
		static std::string errorToString(GLenum error);
		std::map<GLuint, int> activeTextures;
		int TextureCount;
};

#endif // OPENGLUTILS_H
