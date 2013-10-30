#include "calculatevertexnormalsworker.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/PolygonMesh.h"
CalculateVertexNormalsWorker::CalculateVertexNormalsWorker(PolygonMesh * mesh,
														   unsigned int min,
														   unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}
void CalculateVertexNormalsWorker::doWork(){
	typedef std::vector<vis::Polygon*>::size_type polygonSizeType;
	std::vector<vis::Vertex*>& vertices = pmesh->getVertices();
	for(unsigned int i = rangeMin;i<rangeMax;i++){
		glm::vec3 normal(0.0f,0.0f,0.0f);
		vis::Vertex* currentVertex = vertices[i];
		std::vector<vis::Polygon*>& vertexPolygons = currentVertex->getVertexPolygons();
		polygonSizeType nPolygons = vertexPolygons.size();
		int np = 0;
		if(nPolygons > 0){
			for( polygonSizeType j = 0; j < nPolygons; j++ ){
				if(vertexPolygons[j]->isAtSurface()){
					normal += vertexPolygons[j]->getNormal();
					np++;
				}
			}
			if(np>0){
				normal = glm::normalize(normal);
			}
		}
		currentVertex->setNormal(normal);
	}
}
