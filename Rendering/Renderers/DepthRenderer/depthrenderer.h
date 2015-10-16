#ifndef DEPTHRENDERER_H
#define DEPTHRENDERER_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
class FrameBufferObjectHandle;
class DepthRendererConfig;
class DepthRenderer: public Renderer
{
	public:
		DepthRenderer();
		virtual ~DepthRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
        BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
	private:
		bool resize(glm::vec2);
		GLuint program1;
		GLuint program2;
		FrameBufferObjectHandle* fboHandle;
		GLuint depthTextureHandle;
		glm::vec2 viewportSize;
		DepthRendererConfig* config;
};

#endif // DEPTHRENDERER_H
