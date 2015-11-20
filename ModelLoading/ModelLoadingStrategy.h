#ifndef MODELLOADINGSTRATEGY_H
#define MODELLOADINGSTRATEGY_H

#include "Exceptions/unknownextensionexception.h"
#include "Exceptions/modelloadingexception.h"
#include <string>
#include <vector>
#include <QThread>
#include "UI/modelloadingprogressdialog.h"
#define N_DEFAULT_THREADS_MODEL_LOADING 8
#define MODEL_LOAD_FUNC_DEC \
	virtual Model* load( std::string filename )
#define MODEL_VALIDATE_FUNC_DEC \
	virtual bool validate( std::string filename )
#define MODEL_LOADING_EXTENDING_CLASS_MINIMAL(x)\
	x();\
	virtual ~x();\
	MODEL_LOAD_FUNC_DEC;\
	MODEL_VALIDATE_FUNC_DEC;

class Model;
class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class LightWeightVertexCloud;
class LightWeightPolygonMesh;
class LightWeightPolyhedronMesh;
struct AcceptedFileFormat;

class ModelLoadingStrategy:public QThread
{
		Q_OBJECT
	public:
		ModelLoadingStrategy();
		ModelLoadingStrategy(std::string fileFormatName, std::string fileFormatExt);
		virtual ~ModelLoadingStrategy();
		MODEL_LOAD_FUNC_DEC = 0;
		MODEL_VALIDATE_FUNC_DEC = 0;
		std::vector<AcceptedFileFormat>& getFileFormats();

		//QThread
		void run();
		void loadModelQThread(std::string filename);
		Model* getLoadedModel();
	signals:
		void modelLoadedSuccesfully();
		void setupProgressBarForNewModel(int,int,int,int);
		void setLoadedVertices(int);
		void setLoadedPolygons(int);
		void setLoadedPolyhedrons(int);
		void stageComplete(int);
		void addMessage(QString);
		void errorLoadingModel(QString);
		void warningLoadingModel(QString);
	protected:
		static void completeVertexPolygonRelations(PolygonMesh* m);
		static void completePolygonPolygonRelations(PolygonMesh* pmesh,
													int nThreads = N_DEFAULT_THREADS_MODEL_LOADING); //needs vertex Polygons relations
		static void completePolygonPolyhedronRelations(PolyhedronMesh* m);
		static void calculateNormalsPolygons(PolygonMesh* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void calculateNormalsVertices(PolygonMesh* m, bool nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void calculateNormalsVertices(LightWeightPolygonMesh* m,bool nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void calculateGeoCenterPolyhedronMesh(PolyhedronMesh* m, int nThread = N_DEFAULT_THREADS_MODEL_LOADING);
		static void fixSurfacePolygonsVerticesOrder(PolyhedronMesh* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void fixSurfacePolygonsVerticesOrder(LightWeightPolyhedronMesh* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void shrinkVertexPolygonRelations(PolygonMesh* m);
		std::vector<AcceptedFileFormat> acceptedFileFormats;
		void completeMesh(PolygonMesh*);
		void completeMesh(PolyhedronMesh*);
		void completeMesh(LightWeightPolygonMesh*);
		void completeMesh(LightWeightPolyhedronMesh*);
		//tools
	private:
		Model* loadedModel;
		std::string modelPath;

};

#endif // MODELLOADINGSTRATEGY_H
