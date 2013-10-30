#include "triangle.h"
#include "Model/Element/Vertex.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/SelectionStrategy.h"
vis::Triangle::Triangle(int newid):Polygon(newid){
	lmin = NULL_SIDE;
	lmax = NULL_SIDE;
	lmid = NULL_SIDE;
}
vis::Triangle::~Triangle(){}

float vis::Triangle::getArea(){
	if(this->area == NULL_AREA){
		//triangle
		float a = getLMax();
		float b = getLMid();
		float c = getLMin();
		float p = (a+b+c)/2.0f;
		area = sqrt(p*(p-a)*(p-b)*(p-c));
		//area = 0.25f*sqrt((a+b+c)*(c-a+b)*(c+a-b)*(a+b-c));

	}
	return this->area;
}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Triangle)

float vis::Triangle::getLMin(){//smallest side
	if(lmin==NULL_SIDE)
		setSides();
	return lmin;
}
float vis::Triangle::getLMax(){//longest side
	if(lmax==NULL_SIDE)
		setSides();
	return lmax;
}
float vis::Triangle::getLMid(){//middle side
	if(lmid==NULL_SIDE)
		setSides();
	return lmid;
}
void vis::Triangle::setSides(){
	lmin = glm::length(vertices[0]->getCoords()-vertices[1]->getCoords());
	lmid = glm::length(vertices[0]->getCoords()-vertices[2]->getCoords());
	lmax = glm::length(vertices[1]->getCoords()-vertices[2]->getCoords());
	if (lmid < lmin)
		std::swap(lmid,lmin);
	if (lmax < lmid)
		std::swap(lmax,lmid);
	if (lmid < lmin)
		std::swap(lmid,lmin);
}

void vis::Triangle::invertVerticesOrder(){
	vis::Vertex* tmp = vertices[0];
	vertices[0] = vertices[2];
	vertices[2] = tmp;
	normal = -normal;
}
