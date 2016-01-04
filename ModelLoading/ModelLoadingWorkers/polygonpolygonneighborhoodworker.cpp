#include "polygonpolygonneighborhoodworker.h"
#include "Model/PolygonMesh.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
PolygonPolygonNeighborhoodWorker::PolygonPolygonNeighborhoodWorker(PolygonMesh * mesh,
																   unsigned int min,
																   unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}
void PolygonPolygonNeighborhoodWorker::doWork(){
	std::vector<vis::Polygon*> &p = pmesh->getPolygons();
	for( unsigned int j = rangeMin; j < rangeMax; j++ ) {
		std::unordered_map<int,int> neighboringPolygons;
		std::vector<vis::Vertex*>& currentVertices = p[j]->getVertices();
		std::vector<vis::Polygon*>& neighborPolygons = p[j]->getNeighborPolygons();
		for( vis::Vertex* vertex : currentVertices ) {
			for( vis::Polygon* polygon : vertex->getVertexPolygons() ){
				if(polygon==p[j])
					continue;
				if((++neighboringPolygons[polygon->getId()])==2){
					//if(pols[h]!=p[j] && !p[j]->isNeighbor(pols[h])){
					neighborPolygons.push_back(polygon);
					//	pols[h]->getNeighborPolygons().push_back(p[j]);
				}

			}
		}
	}
}
