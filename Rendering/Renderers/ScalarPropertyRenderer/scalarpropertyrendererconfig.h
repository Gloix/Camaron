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
struct VScalarDef;
class ScalarPropertyRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
        explicit ScalarPropertyRendererConfig(QWidget *parent = 0);
        virtual ~ScalarPropertyRendererConfig();
		void readConfig();
        bool setBoundsFromModel();

		VScalarDef *selectedScalarDef;
        int coloring_type;
        int inverse_intensity;
		void setModel(RModel* model);
		std::map<VScalarDef*, std::vector<float>> selectedBounds;

	private:

		Ui::ScalarPropertyRendererConfig *ui;
		std::vector<VScalarDef*> scalarDefs;
		std::map<int,VScalarDef*> scalarDefIdsMap;
        RModel* rmodel;
		Model* model;
    private slots:
        void loadBoundsFromModel();
		void changedProperty(int index);

};

#endif // SCALARPROPERTYRENDERERCONFIG_H
