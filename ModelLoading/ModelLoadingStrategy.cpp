#include "ModelLoading/ModelLoadingStrategy.h"
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
ModelLoadingStrategy::ModelLoadingStrategy(): QThread(){
	loadedModel = 0;
}
ModelLoadingStrategy::ModelLoadingStrategy(std::string fileFormatName,
										   std::string fileFormatExt): QThread()
{
	loadedModel = 0;
	acceptedFileFormats.push_back(AcceptedFileFormat(fileFormatName,fileFormatExt));
}

ModelLoadingStrategy::~ModelLoadingStrategy()
{
	//dtor
}
void ModelLoadingStrategy::completeVertexPolygonRelations(PolygonMesh* pmesh){
	for( vis::Polygon* polygon : pmesh->getPolygons() ) {
		for( vis::Vertex* vertex : polygon->getVertices()){
			vertex->getVertexPolygons().push_back(polygon);
		}
	}
}
//needs vertex polygon relations
void ModelLoadingStrategy::completePolygonPolygonRelations(PolygonMesh* pmesh,
														   int nThreads){
	if(nThreads<=1 || pmesh->getPolygonsCount()<10000)
		nThreads = 1;

	//THREADED
	int nPolygons = pmesh->getPolygonsCount();
	int nBlockSize = nPolygons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolygons;
		workers.push_back(new PolygonPolygonNeighborhoodWorker(pmesh,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);

}

std::vector<AcceptedFileFormat>& ModelLoadingStrategy::getFileFormats(){
	return acceptedFileFormats;
}

void ModelLoadingStrategy::completePolygonPolyhedronRelations(PolyhedronMesh* m){
	for(vis::Polyhedron* polyhedron : m->getPolyhedrons()){
		for(vis::Polygon* polygon : polyhedron->getPolyhedronPolygons()){
			polygon->addPolyhedron(polyhedron);
		}
	}
}
void ModelLoadingStrategy::calculateNormalsPolygons(PolygonMesh* m, int nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	//THREADED
	int nPolygons = m->getPolygonsCount();
	int nBlockSize = nPolygons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolygons;
		workers.push_back(new CalculatePolygonNormalsWorker(m,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);

}
void ModelLoadingStrategy::calculateNormalsVertices(LightWeightPolygonMesh* m,bool nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	for( vis::LWPolygon* lwPolygon : m->getPolygons() ){
		if(lwPolygon->isAtSurface()){
			std::vector<vis::LWVertex*>& currentVertices = lwPolygon->getVertices();
			for( vis::LWVertex* lwVertex : currentVertices ){
				lwVertex->setNormal(lwVertex->getNormal()+lwPolygon->getNormal());
			}
		}
	}
	//THREADED
	int nvertices = m->getVerticesCount();
	int nBlockSize = nvertices/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nvertices;
		workers.push_back(new CalculateVertexNormalsLightWeightModelWorker(m,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	return;

}

void ModelLoadingStrategy::calculateNormalsVertices(PolygonMesh* m,bool nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	//THREADED HEAVY WEIGHT
	int nvertices = m->getVerticesCount();
	int nBlockSize = nvertices/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nvertices;
		workers.push_back(new CalculateVertexNormalsWorker(m,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);
}
void ModelLoadingStrategy::calculateGeoCenterPolyhedronMesh(PolyhedronMesh* m,
															int nThreads){
	if(nThreads<=1 || m->getPolyhedronsCount()<10000)
		nThreads = 1;
	//THREADED
	int nPolyhedrons = m->getPolyhedronsCount();
	int nBlockSize = nPolyhedrons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolyhedrons;
		workers.push_back(new CalculatePolyhedronGeocenterWorker(m,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);

}
//needs geocenter and normals calculated
void ModelLoadingStrategy::fixSurfacePolygonsVerticesOrder(PolyhedronMesh* m,
														   int nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	//THREADED
	int nPolygons = m->getPolygonsCount();
	int nBlockSize = nPolygons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolygons;
		QtWorkerBase* worker = new FixSurfacePolygonsVerticesOrderWorker<PolygonMesh,
				vis::Polygon,
				vis::Polyhedron>(m,from,to);
		workers.push_back(worker);
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);
}
void ModelLoadingStrategy::fixSurfacePolygonsVerticesOrder(
		LightWeightPolyhedronMesh* m, int nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	//THREADED
	int nPolygons = m->getPolygonsCount();
	int nBlockSize = nPolygons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolygons;
		QtWorkerBase* worker = new FixSurfacePolygonsVerticesOrderWorker<LightWeightPolygonMesh,
				vis::LWPolygon,
				vis::LWPolyhedron>(m,from,to);
		workers.push_back(worker);
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);
}

Model* ModelLoadingStrategy::getLoadedModel(){
	Model* temp = loadedModel;
	loadedModel = 0;
	return temp;
}
void ModelLoadingStrategy::run(){
	try{
		loadedModel = 0;
		loadedModel = load(modelPath);
		if(loadedModel){
			emit stageComplete(ModelLoadingProgressDialog::BASE_MODE_READY);
			emit modelLoadedSuccesfully();
		}
	}catch(ExceptionMessage& e){
		emit errorLoadingModel(QString::fromStdString(e.getMessage()));
	}catch(std::bad_alloc &ba){
		emit errorLoadingModel( QString("Not enough RAM to load the model, try the x64 version"));
	}catch(std::exception& e){
		emit errorLoadingModel( QString(e.what()));
	}
}
void ModelLoadingStrategy::loadModelQThread(std::string filename){
	modelPath = filename;
	this->start();
}

void ModelLoadingStrategy::completeMesh(PolygonMesh* mesh){
	completeVertexPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
	completePolygonPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
	calculateNormalsPolygons(mesh);
	calculateNormalsVertices(mesh);
	emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
}

void ModelLoadingStrategy::completeMesh(PolyhedronMesh* mesh){
	completeVertexPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
	completePolygonPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
	completePolygonPolyhedronRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYHEDRON_R);
	calculateGeoCenterPolyhedronMesh(mesh);
	emit stageComplete(ModelLoadingProgressDialog::POLYHEDRON_GEOCENTER_CALCULATED);
	calculateNormalsPolygons(mesh);
	fixSurfacePolygonsVerticesOrder(mesh);
	emit stageComplete(ModelLoadingProgressDialog::FIXED_SURFACE_POLYGONS_VERTICES_ORDER);
	calculateNormalsVertices(mesh);
	emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
}


void ModelLoadingStrategy::completeMesh(LightWeightPolygonMesh* mesh){
	calculateNormalsVertices(mesh);
	emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
}

void ModelLoadingStrategy::completeMesh(LightWeightPolyhedronMesh* mesh){
	fixSurfacePolygonsVerticesOrder(mesh);
	emit stageComplete(ModelLoadingProgressDialog::FIXED_SURFACE_POLYGONS_VERTICES_ORDER);
	calculateNormalsVertices(mesh);
	emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
}

