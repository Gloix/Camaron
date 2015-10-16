#ifndef ISOLINERENDERERCONFIG_H
#define ISOLINERENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Utils/chararrayscanner.h"
#include "Model/PolygonMesh.h"
#include "Rendering/Renderers/baserendererconfig.h"

namespace Ui {
class IsolineRendererConfig;
}

class IsolineRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
        explicit IsolineRendererConfig(QWidget *parent = 0);
        ~IsolineRendererConfig();
        glm::vec4 gradientStartColor;
        glm::vec4 gradientEndColor;
		glm::vec4 wireframeColor;
        VScalarDef* selectedScalarDef;
        std::vector<float> isolinesSteps;
		int wireFrameOption;
		void readConfig();
        void setModel(RModel*);
    public slots:
        void changeScalarPropFunc(int index);
	private:
		static const int NO_WIREFRAME = 0;
		static const int COMPLETE_WIREFRAME = 1;
		static const int SURFACE_WIREFRAME = 2;
        Ui::IsolineRendererConfig *ui;
        std::vector<VScalarDef*> scalarDefs;
        // Keep a model pointer to keep track of when the model changes.
        Model* model;
        std::map<int,VScalarDef*> scalarDefIdsMap;
};

#endif // ISOLINERENDERERCONFIG_H
