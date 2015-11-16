#ifndef ADDITIONALEDGESRENDERER_H
#define ADDITIONALEDGESRENDERER_H
#include "Rendering/Renderer.h"
#include <GL/glew.h>

class AdditionalEdgesRenderer:public Renderer
{
	public:
		AdditionalEdgesRenderer();
		virtual ~AdditionalEdgesRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
	private:
		GLuint theProgram;
};

#endif // ADDITIONALEDGESRENDERER_H
