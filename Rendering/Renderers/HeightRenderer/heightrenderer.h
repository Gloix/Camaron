#ifndef HEIGHTRENDERER_H
#define HEIGHTRENDERER_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/HeightRenderer/heightrendererconfig.h"

class HeightRenderer: public Renderer
{
	public:
        HeightRenderer();
        virtual ~HeightRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
        BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);

	private:

        HeightRendererConfig* config;

        GLuint program;
};
#endif // HEIGHTRENDERER_H
