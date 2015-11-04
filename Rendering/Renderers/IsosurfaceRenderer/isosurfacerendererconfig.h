#ifndef ISOSURFACERENDERERCONFIG_H
#define ISOSURFACERENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Utils/chararrayscanner.h"
#include "Model/PolygonMesh.h"
#include "Rendering/Renderers/baserendererconfig.h"

struct RModelVScalarDef;
namespace Ui {
class IsosurfaceRendererConfig;
}

class IsosurfaceRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
        explicit IsosurfaceRendererConfig(QWidget *parent = 0);
        ~IsosurfaceRendererConfig();
        glm::vec4 gradientStartColor;
        glm::vec4 gradientEndColor;
		glm::vec4 wireframeColor;
		RModelVScalarDef* selectedScalarDef;
        std::vector<float> isolevels;
		int wireFrameOption;
		void readConfig();
        void setModel(RModel*);
    public slots:
        void changeScalarPropFunc(int index);
        void changeInputType(int tabIndex);
        void sweepValueChanged(int value);
    signals:
        void applyChangesPushButton();
	private:
		static const int NO_WIREFRAME = 0;
		static const int COMPLETE_WIREFRAME = 1;
		static const int SURFACE_WIREFRAME = 2;
        static const int INPUT_VALUES_LIST = 0;
        static const int INPUT_SWEEP = 1;
        int currentInputType = 0;
        Ui::IsosurfaceRendererConfig *ui;
		std::vector<RModelVScalarDef*> scalarDefs;
        // Keep a model pointer to keep track of when the model changes.
        Model* model;
		std::map<int,RModelVScalarDef*> scalarDefIdsMap;
};

#endif // ISOSURFACERENDERERCONFIG_H
