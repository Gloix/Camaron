#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "math.h"
#include <iostream>
vis::Vertex::Vertex( int idn, float xn, float yn, float zn, int npos ):
	Element( idn ),
	coords(xn,yn,zn),
	normal(0.0f,0.0f,0.0f){
	pos = (npos==Vertex::NULL_POS)?idn:npos;
}

vis::Vertex::~Vertex() {
}
glm::vec3& vis::Vertex::getCoords(){ return coords; }

int vis::Vertex::getPos(){ return this->pos;}
void vis::Vertex::setPos(int npos){ this->pos = npos;}

float vis::Vertex::getScalarProperty(int i) {
	return this->getProperty(i+SCALAR_PROPERTY_START_INDEX);
}

void vis::Vertex::addScalarProperty(int index, float scalar) {
	this->addProperty(index+SCALAR_PROPERTY_START_INDEX, scalar);
}

glm::vec3& vis::Vertex::getNormal(){
	/*if(recalculate || !normalCalculated){
		normal = glm::vec3(0.0f,0.0f,0.0f);
		typedef std::vector<vis::Polygon*>::size_type local_size_type;
		local_size_type nPolygons = this->polygons.size();
		int np = 0;
		if(nPolygons > 0){
			normalCalculated = true;
			for( local_size_type i = 0; i < nPolygons; i++ ){
				if(polygons[i]->isAtSurface()){
					this->normal += this->polygons[i]->getNormal();
					np++;
				}
			}
			if(np>0){
				this->normal = glm::normalize(normal);
			}
		}


	}*/
	return normal;
}
void vis::Vertex::setNormal(glm::vec3 newNormal){
	normal = newNormal;
}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Vertex)

bool vis::Vertex::isAtSurface(){
	typedef std::vector<vis::Polygon*>::size_type local_size_type;
	for(local_size_type i = 0; i<polygons.size();i++)
		if(polygons[i]->isAtSurface())
			return true;
	return false;
}
std::vector<vis::Polygon*>& vis::Vertex::getVertexPolygons(){
	return this->polygons;
}

glm::vec3 vis::Vertex::getGeometricCenter(){
	return coords;
}

