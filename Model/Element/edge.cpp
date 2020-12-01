#include "Edge.h"
#include "Model/Element/Vertex.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/SelectionStrategy.h"

vis::Edge::Edge(int idn, vis::Vertex* v0, vis::Vertex* v1):
	Element(idn),vertex0(v0),vertex1(v1){}

vis::Edge::Edge(int idn, vis::Vertex* v0, vis::Vertex* v1, glm::vec3 color):
	Element(idn),
	vertex0(v0),
	vertex1(v1),
	color(color){

}

vis::Vertex* vis::Edge::getVertex0() {
	return vertex0;
}
vis::Vertex* vis::Edge::getVertex1() {
	return vertex1;
}

void vis::Edge::setColor(glm::vec3 color) {
	this->color = color;
}
glm::vec3& vis::Edge::getColor() {
	return this->color;
}

bool vis::Edge::isAtSurface() {
	return true;
}

glm::vec3 vis::Edge::getGeometricCenter() {
	return (vertex0->getCoords()+vertex1->getCoords())/2.0f;
}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Edge)
