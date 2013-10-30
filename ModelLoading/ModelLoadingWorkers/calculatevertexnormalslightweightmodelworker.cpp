#include "calculatevertexnormalslightweightmodelworker.h"
#include "Model/lightweightpolygonmesh.h"
#include "Model/LWElements/lwvertex.h"
CalculateVertexNormalsLightWeightModelWorker::
CalculateVertexNormalsLightWeightModelWorker(LightWeightPolygonMesh * mesh,
											 unsigned int min,
											 unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}
void CalculateVertexNormalsLightWeightModelWorker::doWork(){
	std::vector<vis::LWVertex*>& vertices = pmesh->getVertices();
	for(unsigned int i = rangeMin;i<rangeMax;i++){
		vis::LWVertex* current = vertices[i];
		if(glm::length(current->getNormal())>0.0f){
			current->setNormal(glm::normalize(current->getNormal()));
		}
	}
}
