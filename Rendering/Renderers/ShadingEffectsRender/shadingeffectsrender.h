#ifndef SHADINGEFFECTSRENDER_H
#define SHADINGEFFECTSRENDER_H
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/ShadingEffectsRender/shadingeffectsrenderconfig.h"
#include <GL/glew.h>
class ShadingEffectsRender:public Renderer
{
	public:
		ShadingEffectsRender();
		virtual ~ShadingEffectsRender();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		QWidget* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel* rmodel = (RModel*)0);
	private:
		ShadingEffectsRenderConfig* config;
		GLuint theProgram;
};

#endif // SHADINGEFFECTSRENDER_H
