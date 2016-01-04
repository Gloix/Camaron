#include "lwpolygon.h"
#include "lwvertex.h"
vis::LWPolygon::LWPolygon(int idn):
	LWElement(idn)
{
	polyhedrons[0] = (vis::LWPolyhedron*)0;
	polyhedrons[1] = (vis::LWPolyhedron*)0;
}
vis::LWPolygon::~LWPolygon(){}
std::vector<vis::LWVertex*>& vis::LWPolygon::getVertices(){
	return vertices;
}

int vis::LWPolygon::getVerticesCount(){
	return vertices.size();
}

glm::vec3 vis::LWPolygon::getGeometricCenter(){
	glm::vec3 center(0.0f,0.0f,0.0f);
	for( vis::LWVertex* lwVertex : vertices)
		center += lwVertex->getCoords();
	if(vertices.size()!=0u)
		center /= (float)vertices.size();
	return center;
}

glm::vec3 vis::LWPolygon::getNormal(){
	if( this->getVerticesCount() > 2 ) {
		return glm::normalize(glm::cross(vertices[1]->getCoords()-vertices[0]->getCoords(),
										 vertices[2]->getCoords()-vertices[1]->getCoords()));
	}
	return glm::vec3(0.0f,0.0f,0.0f);
}

bool vis::LWPolygon::isAtSurface(){
	return polyhedrons[1] == 0;
}

void vis::LWPolygon::addPolyhedron(vis::LWPolyhedron* pol){
	if(!polyhedrons[0])
		polyhedrons[0] = pol;
	else
		polyhedrons[1] = pol;
}

vis::LWPolyhedron* vis::LWPolygon::getNeighborPolyhedron(vis::LWPolyhedron* pol){
	if(polyhedrons[0] == pol)
		return polyhedrons[1];
	return polyhedrons[0];
}
vis::LWPolyhedron** vis::LWPolygon::getNeighborPolyhedron(){
	return polyhedrons;
}

void vis::LWPolygon::invertVerticesOrder(){
	std::vector<vis::LWVertex *> tmp;
	for(int i = (int)vertices.size()-1;i>=0;i--)
		tmp.push_back(vertices[i]);
	vertices.swap(tmp);
}
