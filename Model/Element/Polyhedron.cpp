#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/SelectionStrategy.h"

vis::Polyhedron::Polyhedron( int newid ): Element( newid )
{
	//ctor
	area = NULL_VALUE;
	volume = NULL_VALUE;
}

vis::Polyhedron::~Polyhedron()
{
	//dtor
}

int vis::Polyhedron::getPolygonsCount() {
	return polygons.size();
}
std::vector<vis::Polygon*>& vis::Polyhedron::getPolyhedronPolygons() {
	return polygons;
}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Polyhedron)

void vis::Polyhedron::setGeoCenter(glm::vec3 g){
	geoCenter = g;
}
void vis::Polyhedron::calculateGeoCenter(){
	geoCenter = glm::vec3(0.0f,0.0f,0.0f);
	int n = 0;
	for( vis::Polygon* polygon : polygons){
		for( vis::Vertex* vertex : polygon->getVertices())
			geoCenter += vertex->getCoords();
		n += polygon->getVertices().size();
	}
	geoCenter /= (float)n;
}

glm::vec3 vis::Polyhedron::getInwardNormal(vis::Polygon* p){
	glm::vec3 currentFaceNormal = p->getNormal();
	glm::vec3 inwardVector = geoCenter - p->getVertices()[0]->getCoords();
	return (glm::dot(currentFaceNormal,inwardVector)<0)?
				-currentFaceNormal:currentFaceNormal;
}

glm::vec3 vis::Polyhedron::getOutwardNormal(vis::Polygon* p){
	glm::vec3 currentFaceNormal = p->getNormal();
	glm::vec3 inwardVector = geoCenter - p->getVertices()[0]->getCoords();
	return (glm::dot(currentFaceNormal,inwardVector)>0)?
				-currentFaceNormal:currentFaceNormal;
}

float vis::Polyhedron::getArea(){
	if(area == NULL_VALUE){
		area = 0.0f;
		for(vis::Polygon* polygon : polygons )
			area += polygon->getArea();
	}
	return area;
}

float vis::Polyhedron::getVolume(){
	if(volume == NULL_VALUE){
		volume = 0.0f;
		for( vis::Polygon* polygon : polygons){
			glm::vec3 inwardNormal = getInwardNormal(polygon);
			glm::vec3 inwardVectorF = geoCenter -
									  polygon->getVertices()[0]->getCoords();
			//distance: distance from geoCenter to polygon plane
			float distance = glm::dot(inwardNormal,
									  inwardVectorF);//no need to divide by normal module (normal is normalized)
			volume += distance*polygon->getArea()/3.0f;
		}
	}
	return volume;
}
bool vis::Polyhedron::isAtSurface(){
	for( vis::Polygon* polygon : polygons )
		if(polygon->isAtSurface())
			return true;
	return false;
}
glm::vec3 vis::Polyhedron::getGeometricCenter(){
	return geoCenter;
}
