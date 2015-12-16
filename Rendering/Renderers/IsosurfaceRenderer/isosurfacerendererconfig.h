#ifndef ISOSURFACERENDERERCONFIG_H
#define ISOSURFACERENDERERCONFIG_H

#include <QWidget>
#include <memory>
#include <glm/glm.hpp>
#include "Utils/chararrayscanner.h"
#include "Model/PolygonMesh.h"
#include "Rendering/Renderers/baserendererconfig.h"
#include "Model/modelvisitor.h"

template <typename T> class RModelPropertyFieldDef;
class PolyhedronMesh;
namespace Ui {
class IsosurfaceRendererConfig;
}

class IsosurfaceRendererConfig : public BaseRendererConfig, public ModelVisitor
{
		Q_OBJECT
		
	public:
		explicit IsosurfaceRendererConfig(QWidget *parent = 0);
		~IsosurfaceRendererConfig();
		glm::vec4 gradientStartColor;
		glm::vec4 gradientEndColor;
		glm::vec4 wireframeColor;
		std::shared_ptr<RModelPropertyFieldDef<ScalarFieldDef>> selectedScalarRModelDef;
		std::vector<float> isolevels;
		int wireFrameOption;
		void readConfig();
		void setRModel(RModel*);
		virtual void visit(PolyhedronMesh* model);
	public slots:
		void changeScalarPropFunc(int index);
		void changeInputType(int tabIndex);
		void sweepValueChanged(int value);
	signals:
		void applyChangesPushButton();
	private:
		void loadScalarDefs();
		static const int NO_WIREFRAME = 0;
		static const int COMPLETE_WIREFRAME = 1;
		static const int SURFACE_WIREFRAME = 2;
		static const int INPUT_VALUES_LIST = 0;
		static const int INPUT_SWEEP = 1;
		int currentInputType = 0;
		Ui::IsosurfaceRendererConfig *ui;
		std::vector<std::shared_ptr<ScalarFieldDef>> scalarDefs;
		// Keep a model pointer to keep track of when the model changes.
		RModel* rmodel;
		PolyhedronMesh* model;
		std::map<int,std::shared_ptr<PropertyFieldDef>> scalarDefIdsMap;
};

#endif // ISOSURFACERENDERERCONFIG_H
