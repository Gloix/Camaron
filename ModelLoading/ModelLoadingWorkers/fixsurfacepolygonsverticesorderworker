#ifndef FIXSURFACEPOLYGONSVERTICESORDERWORKER_H
#define FIXSURFACEPOLYGONSVERTICESORDERWORKER_H

#include "Utils/qtworkerbase.h"
class PolyhedronMesh;
template<class MESH_TYPE, class POLYGON_TYPE, class POLYHEDRON_TYPE>
class FixSurfacePolygonsVerticesOrderWorker: public QtWorkerBase
{
	public:
		FixSurfacePolygonsVerticesOrderWorker(MESH_TYPE* mesh,
											  unsigned int min,
											  unsigned int max);

		void doWork();
	private:
		MESH_TYPE* pmesh;
		unsigned int rangeMin,rangeMax;
};

template<class MESH_TYPE, class POLYGON_TYPE, class POLYHEDRON_TYPE>
FixSurfacePolygonsVerticesOrderWorker<MESH_TYPE,POLYGON_TYPE,POLYHEDRON_TYPE>::
FixSurfacePolygonsVerticesOrderWorker(MESH_TYPE * mesh,
									  unsigned int min,
									  unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}

template<class MESH_TYPE, class POLYGON_TYPE, class POLYHEDRON_TYPE>
void FixSurfacePolygonsVerticesOrderWorker<MESH_TYPE,POLYGON_TYPE,POLYHEDRON_TYPE>::
doWork(){
	std::vector<POLYGON_TYPE*>& polygons = pmesh->getPolygons();
	for(unsigned int i = rangeMin;i<rangeMax;i++){
		POLYGON_TYPE* current = polygons[i];
		if(current->isAtSurface()){
			POLYHEDRON_TYPE* pol = current->getNeighborPolyhedron((POLYHEDRON_TYPE*)0);
			if(pol){
				glm::vec3 currentFaceNormal = current->getNormal();
				glm::vec3 inwardVector = pol->getGeometricCenter()-
										 current->getVertices()[0]->getCoords();
				if(glm::dot(currentFaceNormal,inwardVector)>0){
					current->invertVerticesOrder();
				}
			}
		}
	}
}

#endif // FIXSURFACEPOLYGONSVERTICESORDERWORKER_H
