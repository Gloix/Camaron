#include "Utils/Printer.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "SelectionStrategies/Selection.h"
#include <iostream>
#include <unordered_map>

Printer::Printer() {}
Printer::~Printer() {}


void Printer::print( vis::Vertex* v ) {
	std::cout << "Vertex " << v->getId() << "(" << v->getCoords().x << ", " << v->getCoords().y << ", " << v->getCoords().z << ")"<< std::endl;
	std::cout << "Polygons using this vertex: ";
/*	std::vector<vis::Polygon*> &vecPol = v->getVertexPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i != vecPol.size(); i++ ) {
		std::cout << vecPol[i]->getId() << ", ";
	}
	std::cout << std::endl;*/

}
void Printer::print( vis::Polygon* p ) {
	std::cout << "Polygon " << p->getId() << ":\n";
	std::vector<vis::Vertex*> &vecPol = p->getVertices();
	for( std::vector<vis::Vertex*>::size_type i = 0; i != vecPol.size(); i++ ) {
		std::cout << "\t";
		Printer::print( vecPol[i] );
		std::cout << std::endl;
	}
	std::cout << "Neighboring Polygons: ";
/*	std::vector<vis::Polygon*> &neighbors = p->getNeighborPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i != neighbors.size(); i++ ) {
		std::cout << neighbors[i]->getId() << ", ";
	}*/
	std::cout << std::endl;
}
void Printer::print( vis::Element* p ) {
	std::cout << "Element ID: " << p->getId();
	if(p->isSelected())
		std::cout << " Selected";
	std::cout << std::endl;
}
void Printer::print( Selection* sel ) {
	std::cout << "Selection: " << std::endl;
	std::unordered_map<int, vis::Element*> &selected = sel->getSelectedElements();
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = selected.begin(); It != selected.end(); ++It )
	{
		vis::Element* current = ( *It ).second;
		Printer::print(current);
	}

}
void Printer::print(glm::mat4& matrix){
	for(int i = 0 ; i<4; i++){
		std::cout << matrix[i].x << ", " << matrix[i].y << ", " << matrix[i].z << ", " << matrix[i].w << std::endl;
	}
}
void Printer::print(glm::vec4& v){
	std::cout << "glm::vec4(" << v.x << ", " <<  v.y << ", " <<  v.z << ", " << v.w << ")" << std::endl;
}
void Printer::print(glm::vec3& v){
	std::cout << "glm::vec3(" << v.x << ", " <<  v.y << ", " <<  v.z << ")" << std::endl;
}
