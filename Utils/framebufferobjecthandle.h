#ifndef FRAMEBUFFEROBJECTHANDLE_H
#define FRAMEBUFFEROBJECTHANDLE_H

#include <GL/glew.h>
#include <vector>

struct ColorRenderBufferData{
	public:
		GLenum internalformat;
		GLenum attachment;
};
struct TextureBufferData{
	public:
		TextureBufferData(GLuint a,GLenum b,GLenum c){
			textureHandle = a; attachment = b; textureTarget = c;
		}
		GLuint textureHandle;
		GLenum attachment;
		GLenum textureTarget;
};

class FrameBufferObjectHandle{
	public:
		FrameBufferObjectHandle(int width, int height, bool textureBuffer);
		FrameBufferObjectHandle(int width, int height, std::vector<ColorRenderBufferData> colorRenderBuffers);
		FrameBufferObjectHandle(int width, int height, std::vector<TextureBufferData> colorRenderBuffers);
		FrameBufferObjectHandle(int width, int height, ColorRenderBufferData data);
		FrameBufferObjectHandle(int width, int height, TextureBufferData data);
		~FrameBufferObjectHandle();
		void setNewSize(int w, int h);
		bool bind();
		bool unbind();
		bool isOk();
		GLuint getTextureDefaultHandle();
		GLuint getDepthStencilTextureHandle();
		bool attachTexture(TextureBufferData, bool binded = false);
	private:
		GLuint textureDefaultHandle;
		std::vector<GLuint> renderBufferColors;
		GLuint frameTextureBuffer;
		GLuint depthStencilTextureHandle;
		int width, height;
		bool ok;
		void initialize(int w, int h,
						std::vector<ColorRenderBufferData> colorRenderBuffers,
						std::vector<TextureBufferData> textureBufferDatas);
		GLuint currentFBO;
};
#endif // FRAMEBUFFEROBJECTHANDLE_H
