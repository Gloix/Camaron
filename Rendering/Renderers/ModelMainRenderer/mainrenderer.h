#ifndef MAINRENDERER_H
#define MAINRENDERER_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/ModelMainRenderer/mainrendererconfig.h"
class FrameBufferObjectHandle;
class MainRenderer: public Renderer
{
	public:
		MainRenderer();
		virtual ~MainRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		void drawWithDepthPeeling(RModel*);
		QWidget* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool resizeFBO(RModel *);
	private:
		GLuint theProgram;
		GLuint textureShaderProgram;
		GLuint depthPeelingProgram;
		glm::vec2 fboSize;
		FrameBufferObjectHandle* fboHandle;
		GLuint depthStencilTextureHandle;
		GLuint depthStencilTextureHandle2;
		GLuint finalResultTexture;
		GLuint currentLayerTexture;
		MainRendererConfig* config;
};
#endif // MAINRENDERER_H
