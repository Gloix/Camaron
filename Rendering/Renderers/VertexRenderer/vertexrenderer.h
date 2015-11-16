#ifndef VERTEXRENDERER_H
#define VERTEXRENDERER_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/VertexRenderer/vertexrendererconfig.h"
class VertexRenderer:public Renderer
{
	public:
		VertexRenderer();
		virtual ~VertexRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		bool rmodelChanged(RModel* rmodel);
		void applyConfigChanges(RModel * = (RModel*)0);
	private:
		GLuint theProgram;
		VertexRendererConfig* config;
};

#endif // VERTEXRENDERER_H
