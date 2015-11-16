#ifndef SCALARPROPERTYRENDERER_H
#define SCALARPROPERTYRENDERER_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/ScalarPropertyRenderer/scalarpropertyrendererconfig.h"

class ScalarPropertyRenderer: public Renderer
{
	public:
		ScalarPropertyRenderer();
		virtual ~ScalarPropertyRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);

	private:

		ScalarPropertyRendererConfig* config;

		GLuint program;
};
#endif // SCALARPROPERTYRENDERER_H
