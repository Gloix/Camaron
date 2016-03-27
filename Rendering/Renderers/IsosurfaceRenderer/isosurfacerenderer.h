#ifndef ISOSURFACERENDERER_H
#define ISOSURFACERENDERER_H
#include "Rendering/Renderer.h"
#include <GL/glew.h>
#include "Rendering/Renderers/stepsrendererconfig.h"

class IsosurfaceRenderer:public Renderer
{
	public:
		IsosurfaceRenderer();
		virtual ~IsosurfaceRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
	private:
		bool rModelChanged;
		GLuint renderProgram;
		GLuint generateProgram;
		std::vector<GLuint> isosurfacesBuffers;
		std::vector<long> isosurfaceBuffersGeomCounts;
		GLuint triTableTex;
		StepsRendererConfig* config;
		std::vector<float> lastConfigScalarLevels;
		int lastConfigElementDrawnOption;
		void generateIsosurface(RModel* rmodel, std::vector<float>& values);
		bool buildIsosurfaceRenderProgram();
		bool buildIsosurfaceGenerationProgram();
};

#endif // ISOSURFACERENDERER_H
