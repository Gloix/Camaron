#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/lightweightpolygonmesh.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/lightweightpolygonmesh.h"
#include "Model/lightweightpolyhedronmesh.h"
#include "Model/LWElements/lwpolyhedron.h"
#include "Model/LWElements/lwpolygon.h"
#include "Model/Model.h"
#include <unordered_map>
#include "ModelLoading/ModelLoadingWorkers/polygonpolygonneighborhoodworker.h"
#include "ModelLoading/ModelLoadingWorkers/calculatepolygonnormalsworker.h"
#include "ModelLoading/ModelLoadingWorkers/calculatevertexnormalsworker.h"
#include "ModelLoading/ModelLoadingWorkers/calculatepolyhedrongeocenterworker.h"
#include "ModelLoading/ModelLoadingWorkers/fixsurfacepolygonsverticesorderworker.h"
#include "ModelLoading/ModelLoadingWorkers/calculatevertexnormalslightweightmodelworker.h"
#include "Utils/qtutils.h"
#include "Utils/fileutils.h"
#include "PropertyFieldLoading/propertyfielddef.h"

PropertyFieldLoadingStrategy::PropertyFieldLoadingStrategy(): QThread(){
}
PropertyFieldLoadingStrategy::PropertyFieldLoadingStrategy(std::string fileFormatName,
										   std::string fileFormatExt): QThread()
{
	acceptedFileFormats.push_back(AcceptedFileFormat(fileFormatName,fileFormatExt));
}

PropertyFieldLoadingStrategy::~PropertyFieldLoadingStrategy()
{
	//dtor
}

std::vector<AcceptedFileFormat>& PropertyFieldLoadingStrategy::getFileFormats(){
	return acceptedFileFormats;
}

void PropertyFieldLoadingStrategy::run(){
	try{
		load(propertyFieldPath, model, this->selected);
		emit propertyFieldsLoadedSuccesfully();
	}catch(ExceptionMessage& e){
		emit errorLoadingPropertyField(QString::fromStdString(e.getMessage()));
	}catch(std::bad_alloc &ba){
		emit errorLoadingPropertyField( QString("Not enough RAM to load the model, try the x64 version"));
	}catch(std::exception& e){
		emit errorLoadingPropertyField( QString(e.what()));
	}
}
void PropertyFieldLoadingStrategy::loadPropertyFieldQThread(std::string propertyFieldPath, Model* model, std::vector<std::shared_ptr<PropertyFieldDef>> selected){
	this->propertyFieldPath = propertyFieldPath;
	this->model = model;
	this->selected = selected;
	this->start();
}
