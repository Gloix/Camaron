#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Utils/PolygonUtils.h"
vis::Polygon::Polygon( int newid ):
	Element( newid ),
	normal(0.0f,0.0f,0.0f)
{

	area = NULL_AREA;
	polyhedrons[0] = (vis::Polyhedron*)0;
	polyhedrons[1] = (vis::Polyhedron*)0;
}

vis::Polygon::~Polygon()
{
}
int vis::Polygon::getVerticesCount() {
	return vertices.size();
}
std::vector<vis::Vertex*>& vis::Polygon::getVertices() {
	return vertices;
}
std::vector<vis::Polygon *>& vis::Polygon::getNeighborPolygons(){
	return neighborPolygons;
}

void vis::Polygon::calculateNormal(){
	if( this->getVerticesCount() > 2 ) {
		normal = glm::normalize(glm::cross(vertices[1]->getCoords()-vertices[0]->getCoords(),
										   vertices[2]->getCoords()-vertices[1]->getCoords()));
	}
}

glm::vec3& vis::Polygon::getNormal(){
	return this->normal;
}
float vis::Polygon::getArea(){
	if(this->area == NULL_AREA){
		float totalArea = 0.0;
		if( vertices.size() < 3 )
			this->area = totalArea;
		for( std::vector<Vertex*>::size_type i = 1; i < vertices.size() - 1; i++ ) {
			glm::vec3 result = glm::cross(vertices[0]->getCoords()-vertices[i]->getCoords(),
										  vertices[i]->getCoords()-vertices[i+1]->getCoords());
			float normalDirection = glm::dot(result,this->getNormal());
			totalArea += ((normalDirection>0)?1.0:-1.0)*sqrt(glm::dot(result,result)) / 2.0;
		}

		float calculatedArea = (totalArea>0.0)?totalArea:-totalArea;
#ifdef DEBUG_MOD
		//std::cout << "[PolygonUtils::area] Polygon(id =  " << p->getId() << "), Area = " << calculatedArea << std::endl;
#endif
		this->area = calculatedArea;
	}
	return this->area;
}
bool vis::Polygon::isNeighbor(Polygon* p){
	for( std::vector<Polygon*>::size_type i = 0; i < neighborPolygons.size(); i++ )
		if(neighborPolygons[i] == p)
			return true;
	return false;
}


DOUBLE_DISPATCH_ELEMENT_DEF(vis::Polygon)

bool vis::Polygon::isAtSurface(){
	return polyhedrons[1] == 0;
}
void vis::Polygon::addPolyhedron(vis::Polyhedron* pol){
	if(!polyhedrons[0])
		polyhedrons[0] = pol;
	else
		polyhedrons[1] = pol;
}
void vis::Polygon::invertVerticesOrder(){
	std::vector<vis::Vertex *> tmp;
	for(int i = (int)vertices.size()-1;i>=0;i--)
		tmp.push_back(vertices[i]);
	vertices.swap(tmp);
	normal = -normal;//invert normal
}
vis::Polyhedron* vis::Polygon::getNeighborPolyhedron(vis::Polyhedron* pol){
	if(polyhedrons[0] == pol)
		return polyhedrons[1];
	return polyhedrons[0];
}
vis::Polyhedron** vis::Polygon::getNeighborPolyhedron(){
	return polyhedrons;
}
glm::vec3 vis::Polygon::getGeometricCenter(){
	glm::vec3 center(0.0f,0.0f,0.0f);
	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++)
		center += vertices[i]->getCoords();
	if(vertices.size()!=0u)
		center /= (float)vertices.size();
	return center;
}
