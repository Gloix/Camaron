#ifndef RENDERER_H
#define RENDERER_H
#include <string>
#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"

#define RENDERER_WEIGHT_BASE 50.0f
namespace vis{
class Element;
}
class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class RModel;
class Renderer{
	public:
		Renderer(std::string newname);
		virtual ~Renderer();
		virtual void glewIsReadyRenderer();
		virtual void draw(RModel*);
		virtual void rendererSelected(RModel* rmodel);
		virtual bool rmodelChanged(RModel* rmodel);
		virtual BaseRendererConfig* getRendererConfigWidget();
		virtual bool hasRendererConfigWidget();
		virtual void applyConfigChanges(RModel*);
		virtual glm::mat4 getMV(RModel*);
		virtual glm::mat4 getMVP(RModel*);
		virtual glm::mat4 getMVPVP(RModel*);
		//setMatrixOffset() // model vertex transform
		const char* getRendererName() const;
		bool isOk();
		float getRendererWeight();
		void setRendererWeight(float);
		//is Displayed
		virtual bool isValidAsSecondaryRenderer();
		virtual bool isDisplayed(vis::Element*, RModel*);
		virtual glm::vec3 isDisplayedViewPortCheck(vis::Element*, RModel*);
	protected:
		bool ok;
		float rendererWeight;
	private:
		const std::string name;
};

#endif // RENDERER_H
