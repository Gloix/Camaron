#ifndef SCALARPROPERTYRENDERERCONFIG_H
#define SCALARPROPERTYRENDERERCONFIG_H

#include <GL/glew.h>
#include <QWidget>
#include <glm/glm.hpp>
#include "Model/PolygonMesh.h"
#include "Rendering/Renderers/baserendererconfig.h"

namespace Ui {
class ScalarPropertyRendererConfig;
}
class RModel;
struct RModelVScalarDef;
class ScalarPropertyRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
        explicit ScalarPropertyRendererConfig(QWidget *parent = 0);
        virtual ~ScalarPropertyRendererConfig();
		void readConfig();
        bool setBoundsFromModel();

		RModelVScalarDef *selectedScalarDef;
        int coloring_type;
        int inverse_intensity;
		void setModel(RModel* model);
		std::map<RModelVScalarDef*, std::vector<float>> selectedBounds;

	private:

		Ui::ScalarPropertyRendererConfig *ui;
		std::vector<RModelVScalarDef*> scalarDefs;
		std::map<int,RModelVScalarDef*> scalarDefIdsMap;
        RModel* rmodel;
		Model* model;
    private slots:
        void loadBoundsFromModel();
		void changedProperty(int index);

};

#endif // SCALARPROPERTYRENDERERCONFIG_H
