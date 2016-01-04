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
	std::vector<vis::Vertex*>& vertices = pmesh->getVertices();
	for(unsigned int i = rangeMin;i<rangeMax;i++){
		glm::vec3 normal(0.0f,0.0f,0.0f);
		vis::Vertex* currentVertex = vertices[i];
		std::vector<vis::Polygon*>& vertexPolygons = currentVertex->getVertexPolygons();
		int np = 0;
		if(!vertexPolygons.empty()){
			for( vis::Polygon* polygon : vertexPolygons ){
				if(polygon->isAtSurface()){
					normal += polygon->getNormal();
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
