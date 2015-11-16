#ifndef CONVEXGEOMETRYINTERSECTIONRENDERER_H
#define CONVEXGEOMETRYINTERSECTIONRENDERER_H
#include <GL/glew.h>
#include "SelectionStrategies/SelectionStrategy.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionconfigselection.h"
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h"
class ConvexGeometryIntersectionRenderer:public Renderer, public SelectionStrategy
{
	public:
		ConvexGeometryIntersectionRenderer();
		virtual ~ConvexGeometryIntersectionRenderer();
		//Renderer Overide functions
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		BaseRendererConfig* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel * = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);

		//Selection Strategy Override Functions
		virtual bool hasConfigWidget();
		virtual BaseRendererConfig* getConfigWidget();
		virtual bool selectElement( vis::Polyhedron *, Selection* );
		virtual bool selectElement( vis::Polygon *, Selection* );
		bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
		virtual bool isFullFilled( vis::Polygon *);
		virtual bool isFullFilled( vis::Polyhedron *);
		virtual bool isFullFilled( vis::Vertex *);
	private:
		void createMainShader();
		void drawWithMainShader(RModel*);
		//Main Shader attributes
		GLuint theProgram;
		glm::mat4 lastMVMatrix;
		void drawWithConvexGeometryShader(RModel*);
		void parseNewGeometryString(RModel *rmodel);
		bool createInsideAttributeBufferObject(RModel *);

		ConvexGeometryIntersectionRendererConfig* configRenderer;
		ConvexGeometryIntersectionConfigSelection* configSelection;
		GenericConvexGeometry *convexGeometry;
		RModel *rmodel;
		//Selection private methods

		bool selectElementAlgorithm( vis::Element * e, int elementType);
		bool selectElement( vis::Element * e,int elementType,Selection* s );
		bool selectElementFollowModel( vis::Element * e );
		bool selectCPU(vis::Polygon*);
		bool selectCPU(vis::Polyhedron*);
		bool selectCPU(vis::Vertex*);
		inline bool gpuSelectionAlgorithmAvailable();
};
bool ConvexGeometryIntersectionRenderer::gpuSelectionAlgorithmAvailable(){
	return false;
}

#endif // CONVEXGEOMETRYINTERSECTIONRENDERER_H
