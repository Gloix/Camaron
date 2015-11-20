#ifndef PROPERTYFIELDLOADINGSTRATEGY_H
#define PROPERTYFIELDLOADINGSTRATEGY_H

#include "Exceptions/unknownextensionexception.h"
#include "Exceptions/modelloadingexception.h"
#include <string>
#include <vector>
#include <QThread>
#include "UI/modelloadingprogressdialog.h"
#define N_DEFAULT_THREADS_MODEL_LOADING 8
#define PROPERTY_FIELD_LOAD_FUNC_DEC \
	virtual Model* load( std::string filename, Model* model)
#define PROPERTY_FIELD_VALIDATE_FUNC_DEC \
	virtual bool validate( std::string filename )
#define PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(x)\
	x();\
	virtual ~x();\
	PROPERTY_FIELD_LOAD_FUNC_DEC;\
	PROPERTY_FIELD_VALIDATE_FUNC_DEC;

class Model;
class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class LightWeightVertexCloud;
class LightWeightPolygonMesh;
class LightWeightPolyhedronMesh;
struct AcceptedFileFormat;

class PropertyFieldLoadingStrategy:public QThread
{
		Q_OBJECT
	public:
		PropertyFieldLoadingStrategy();
		PropertyFieldLoadingStrategy(std::string fileFormatName, std::string fileFormatExt);
		virtual ~PropertyFieldLoadingStrategy();
		PROPERTY_FIELD_LOAD_FUNC_DEC = 0;
		PROPERTY_FIELD_VALIDATE_FUNC_DEC = 0;
		std::vector<AcceptedFileFormat>& getFileFormats();

		//QThread
		void run();
		void loadPropertyFieldQThread(std::string filename, Model* model);
	signals:
		void propertyFieldLoadedSuccesfully();
		void setupProgressBarForNewPropertyField(int,int,int,int);
		void setLoadedVertices(int);
		void setLoadedPolygons(int);
		void setLoadedPolyhedrons(int);
		void stageComplete(int);
		void addMessage(QString);
		void errorLoadingModel(QString);
		void warningLoadingModel(QString);
	protected:
		std::vector<AcceptedFileFormat> acceptedFileFormats;
		//tools
	private:
		Model* model;
		std::string propertyFieldPath;

};

#endif // PROPERTYFIELDLOADINGSTRATEGY_H
