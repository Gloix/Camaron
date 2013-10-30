#include "plane.h"


Plane::Plane(glm::vec3 norigin, glm::vec3 v1,glm::vec3 v2):
	normal(glm::cross(v1,v2)),
	origin(norigin),
	planeEquation(normal)
{
	d = glm::dot(origin,normal);
}
Plane::Plane(glm::vec3 norigin, glm::vec3 nnormal):
	normal(nnormal),
	origin(norigin),
	planeEquation(nnormal)
{
	d = glm::dot(origin,normal);
}

Plane::~Plane(){

}

glm::vec3 Plane::getPointOfIntersection(Plane& p2,Plane& p3){
	glm::mat3 eqSystem(planeEquation,p2.getPlaneEquation(),p3.getPlaneEquation());
	glm::mat3 eqSystemInverse = glm::inverse(eqSystem);
	glm::vec3 dVector(d,p2.getPlaneEquationConstant(),p3.getPlaneEquationConstant());
	return eqSystemInverse*dVector;
}

bool Plane::isOk(){
	return ok;
}
void Plane::fixNormal(glm::vec3 p){
	if(glm::dot(p,normal)<0.0f){
		normal = -normal;
		planeEquation = glm::vec3(normal.x,normal.y,normal.z);
		d = -d;
	}

}

bool Plane::intersect(Plane& p2,Plane& p3){
	glm::mat3 eqSystem(planeEquation,p2.getPlaneEquation(),p3.getPlaneEquation());
	return glm::determinant(eqSystem)>0;
}
glm::vec3 Plane::getPlaneEquation(){
	return planeEquation;
}

float Plane::getPlaneEquationConstant(){
	return d;
}
glm::vec3 Plane::getNormal(){
	return normal;
}
glm::vec3 Plane::getOrigin(){
	return origin;
}

bool Plane::isLeft(glm::vec3 vec){
	return glm::dot(normal,vec-origin)>=0.0f;
}
