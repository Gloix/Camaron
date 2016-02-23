#ifndef STEPSRENDERERCONFIG_H
#define STEPSRENDERERCONFIG_H

#include <QWidget>
#include <memory>
#include <glm/glm.hpp>
#include "Utils/chararrayscanner.h"
#include "Model/PolygonMesh.h"
#include "Rendering/Renderers/baserendererconfig.h"

template <typename T> class RModelPropertyFieldDef;
class PolyhedronMesh;
namespace Ui {
class StepsRendererConfig;
}

class StepsRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit StepsRendererConfig(QWidget *parent = 0);
		~StepsRendererConfig();
		glm::vec4 gradientStartColor;
		glm::vec4 gradientEndColor;
		glm::vec4 wireframeColor;
		//std::shared_ptr<RModelPropertyFieldDef<ScalarFieldDef>> selectedScalarRModelDef;
		std::shared_ptr<ScalarFieldDef> selectedScalarDef;
		std::vector<float> isolevels;
		int wireFrameOption;
		void readConfig();
		//void setRModel(RModel*);
		void setScalarFields(std::vector<std::shared_ptr<ScalarFieldDef>>&);
		int elementDrawnOption;
	public slots:
		void changeScalarPropFunc(int index);
		void changeInputType(int tabIndex);
		void sweepValueChanged(int value);
	signals:
		void applyChangesPushButton();
	private:
		void readInputIsolevels();
		void loadScalarDefs();
		static const int NO_WIREFRAME = 0;
		static const int COMPLETE_WIREFRAME = 1;
		static const int SURFACE_WIREFRAME = 2;
		static const int INPUT_VALUES_LIST = 0;
		static const int INPUT_SWEEP = 1;

		static const int DRAW_ALL = 0;
		static const int DRAW_ONLY_SELECTED = 3;
		static const int DRAW_ONLY_UNSELECTED = 4;

		int currentInputType = 0;
		Ui::StepsRendererConfig *ui;
		std::vector<std::shared_ptr<ScalarFieldDef>> scalarFields;
		// Keep a model pointer to keep track of when the model changes.
		RModel* rmodel;
		Model* model;
		//std::map<int,std::shared_ptr<PropertyFieldDef>> scalarDefIdsMap;
};

#endif // STEPSRENDERERCONFIG_H
