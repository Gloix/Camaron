#ifndef SCALARPROPERTYRENDERERCONFIG_H
#define SCALARPROPERTYRENDERERCONFIG_H

#include <GL/glew.h>
#include <QWidget>
#include <glm/glm.hpp>
#include "Model/PolygonMesh.h"
#include "Model/modelvisitor.h"
#include "Rendering/Renderers/baserendererconfig.h"
#include "PropertyFieldLoading/propertyfielddefvisitor.h"

class PolyhedronMesh;
template <typename T> class RModelPropertyFieldDef;
namespace Ui {
class ScalarPropertyRendererConfig;
}
class RModel;
class ScalarPropertyRendererConfig : public BaseRendererConfig, public ModelVisitor
{
		Q_OBJECT
		
	public:
		explicit ScalarPropertyRendererConfig(QWidget *parent = 0);
		virtual ~ScalarPropertyRendererConfig();
		void readConfig();
		bool setBoundsFromModel();

		std::shared_ptr<RModelPropertyFieldDef<ScalarFieldDef>> selectedScalarRModelDef;
		int coloring_type;
		int inverse_intensity;
		void setRModel(RModel* model);
		virtual void visit(PolygonMesh* model);
		virtual void visit(PolyhedronMesh* model);
		std::map<PropertyFieldDef*, std::vector<float>> selectedBounds;

	private:
		Ui::ScalarPropertyRendererConfig *ui;
		void loadScalarDefs();
		void onNewModelLoaded();
		std::vector<std::shared_ptr<ScalarFieldDef>> scalarDefs;
		std::map<int,std::shared_ptr<ScalarFieldDef>> scalarDefIdsMap;
		RModel* rmodel;
		Model* model;
	private slots:
		void loadBoundsFromModel();
		void changedProperty(int index);

};

#endif // SCALARPROPERTYRENDERERCONFIG_H
