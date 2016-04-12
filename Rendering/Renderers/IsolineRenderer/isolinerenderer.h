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
		bool rModelChanged;
		GLuint renderProgram;
		GLuint generateProgram;
		GLuint transformFeedback;
		GLuint isolinesBuffer;
		StepsRendererConfig* config;
		float sizeRatio;
		bool buildIsolineRenderProgram();
		bool buildIsolineGenerationProgram();
		std::vector<float> lastConfigScalarLevels;
		std::shared_ptr<ScalarFieldDef> lastSelectedScalarDef;
		int lastConfigElementDrawnOption;
		void generateIsolines(RModel* rmodel, std::vector<float>& values);
};

#endif // ISOLINERENDERER_H
