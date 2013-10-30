#include "calculatepolygonnormalsworker.h"
#include "Model/Element/Polygon.h"
#include "Model/PolygonMesh.h"
CalculatePolygonNormalsWorker::CalculatePolygonNormalsWorker(PolygonMesh * mesh,
															 unsigned int min,
															 unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}
void CalculatePolygonNormalsWorker::doWork(){
	std::vector<vis::Polygon*>& polygons = pmesh->getPolygons();
	for(unsigned int i = rangeMin;i<rangeMax;i++){
		polygons[i]->calculateNormal();
	}
}
