#include "lwvertex.h"

vis::LWVertex::LWVertex( int idn, float x,
						 float y, float z):
	LWElement(idn),
	coords(x,y,z),
	normal(0.0f,0.0f,0.0f){
	usedb = false;
}

vis::LWVertex::~LWVertex(){

}

glm::vec3& vis::LWVertex::getCoords(){
	return coords;
}


glm::vec3& vis::LWVertex::getNormal(){
	return normal;
}

void vis::LWVertex::setNormal(glm::vec3 n){
	normal = n;
}

bool vis::LWVertex::isAtSurface(){
	return glm::length(normal)!=0.0f;
}
bool vis::LWVertex::isUsed(){
	return usedb;
}

void vis::LWVertex::used(){
	usedb = true;
}
