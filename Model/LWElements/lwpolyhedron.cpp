#include "lwpolyhedron.h"
#include "lwvertex.h"
#include "lwpolygon.h"
vis::LWPolyhedron::LWPolyhedron(int idn):
	LWElement(idn)
{
}

vis::LWPolyhedron::~LWPolyhedron(){

}

std::vector<vis::LWPolygon*>& vis::LWPolyhedron::getPolyhedronPolygons(){
	return polygons;
}

int vis::LWPolyhedron::getPolygonsCount(){
	return polygons.size();
}

glm::vec3 vis::LWPolyhedron::getGeometricCenter(){
	glm::vec3 geoCenter(0.0f,0.0f,0.0f);
	int n = 0;
	for(std::vector<vis::LWPolygon*>::size_type i = 0; i< polygons.size();i++){
		std::vector<vis::LWVertex*>& polygonVertices = polygons[i]->getVertices();
		for(std::vector<vis::LWVertex*>::size_type v = 0; v< polygonVertices.size();v++)
			geoCenter += polygonVertices[v]->getCoords();
		n += polygonVertices.size();
	}
	geoCenter /= (float)n;
	return geoCenter;
}

bool vis::LWPolyhedron::isAtSurface(){
	typedef std::vector<vis::LWPolygon*>::size_type local_size_type;
	for(local_size_type i = 0; i<polygons.size();i++)
		if(polygons[i]->isAtSurface())
			return true;
	return false;
}

glm::vec3 vis::LWPolyhedron::getInwardNormal(vis::LWPolygon* p){
	glm::vec3 currentFaceNormal = p->getNormal();
	glm::vec3 inwardVector = getGeometricCenter() - p->getVertices()[0]->getCoords();
	return (glm::dot(currentFaceNormal,inwardVector)<0)?
				-currentFaceNormal:currentFaceNormal;
}

glm::vec3 vis::LWPolyhedron::getOutwardNormal(vis::LWPolygon* p){
	glm::vec3 currentFaceNormal = p->getNormal();
	glm::vec3 inwardVector = getGeometricCenter() - p->getVertices()[0]->getCoords();
	return (glm::dot(currentFaceNormal,inwardVector)>0)?
				-currentFaceNormal:currentFaceNormal;
}
