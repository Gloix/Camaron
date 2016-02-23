#ifndef ISOLINERENDERER_H
#define ISOLINERENDERER_H
#include "Rendering/Renderer.h"
#include <GL/glew.h>
#include "Rendering/Renderers/stepsrendererconfig.h"

class IsolineRenderer:public Renderer
{
	public:
		IsolineRenderer();
		virtual ~IsolineRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
	private:
		//std::shared_ptr<RModelPropertyFieldDef<PropertyFieldDef>> currentRModelPropertyFieldDef;
		GLuint theProgram;
		StepsRendererConfig* config;
		float sizeRatio;
};

#endif // ISOLINERENDERER_H
