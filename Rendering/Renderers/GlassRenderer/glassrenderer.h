#ifndef GLASSRENDERER_H
#define GLASSRENDERER_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
class GlassRendererConfig;
class RModel;

class GlassRenderer:public Renderer
{
	public:
		GlassRenderer();
		virtual ~GlassRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		QWidget* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
	private:
		GLuint theProgram;
		GlassRendererConfig* config;
};

#endif // GLASSRENDERER_H
