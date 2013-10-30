#include "calculatepolyhedrongeocenterworker.h"
#include "Model/Element/Polyhedron.h"
#include "Model/PolyhedronMesh.h"

CalculatePolyhedronGeocenterWorker::CalculatePolyhedronGeocenterWorker(PolyhedronMesh * mesh,
																	   unsigned int min,
																	   unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}
void CalculatePolyhedronGeocenterWorker::doWork(){
	std::vector<vis::Polyhedron*>& polyhedrons = pmesh->getPolyhedrons();
	for(unsigned int i = rangeMin;i<rangeMax;i++){
		polyhedrons[i]->calculateGeoCenter();
	}
}
