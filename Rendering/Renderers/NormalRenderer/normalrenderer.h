#ifndef NORMALRENDERER_H
#define NORMALRENDERER_H
#include "Rendering/Renderer.h"
#include <GL/glew.h>
#include "Rendering/Renderers/NormalRenderer/normalrendererconfig.h"
class NormalRenderer:public Renderer
{
	public:
		NormalRenderer();
		virtual ~NormalRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
        BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
	private:
		GLuint theProgram;
		NormalRendererConfig* config;
		float sizeRatio;
};

#endif // NORMALRENDERER_H
