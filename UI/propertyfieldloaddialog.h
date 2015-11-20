#ifndef PROPERTYFIELDLOADDIALOG_H
#define PROPERTYFIELDLOADDIALOG_H

#include <QWidget>
namespace Ui {
class PropertyFieldLoadDialog;
}

class PropertyFieldLoadDialog : public QWidget
{
		Q_OBJECT
		
	public:
                explicit PropertyFieldLoadDialog(QWidget *parent = 0);
                ~PropertyFieldLoadDialog();
                void setModelName(std::string modelname);
                void setupForPropertyFields(std::vector<string>);
		static const int RMODEL_READY = 6;
		static const int CAMARON_ELEMENTS_CONFIGURATED = 8;
		static const int BASE_MODE_READY = 9;
		//Polygon Mesh operations
		static const int COMPLETED_VERTEX_POLYGON_R = 1;
		static const int COMPLETED_POLYGON_POLYGON_R = 2;
		static const int NORMALS_CALCULATED = 4;
		//Polyhedron Mesh operations
		static const int COMPLETED_POLYGON_POLYHEDRON_R = 3;
		static const int POLYHEDRON_GEOCENTER_CALCULATED = 5;
		static const int FIXED_SURFACE_POLYGONS_VERTICES_ORDER = 7;
	public slots:
		void setupProgressBarForNewModel(int,int,int,int);
		void setLoadedVertices(int);
		void setLoadedPolygons(int);
		void setLoadedPolyhedrons(int);
		void stageComplete(int);
		void addMessage(QString message);
		void displayError(QString message);
		void displayWarning(QString message);
	private:
		Ui::ModelLoadingProgressDialog *ui;
		void resetVertices(int);
		void resetPolygons(int);
		void resetPolyhedrons(int);
		void setVerticesLabels();
		void setPolygonsLabels();
		void setPolyhedronsLabels();
		void updateProgressBar();
		int loadedVertices,loadedPolygons,loadedPolyhedrons;
		int totalVertices,totalPolygons,totalPolyhedrons,totalelements;
		int progressVarValue;
		int progressStages;
		int progressVarMax;
		int modelType;
};

#endif // PROPERTYFIELDLOADDIALOG_H
