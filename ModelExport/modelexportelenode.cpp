#include "modelexportelenode.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Utils/fileutils.h"
#include "Utils/polyhedronutils.h"
#include <iostream>
#include <fstream>
ModelExportEleNode::ModelExportEleNode():
	ModelExportStrategy("Node, Ele","ele")
{
}
ModelExportEleNode::~ModelExportEleNode(){

}
bool ModelExportEleNode::exportModel(PolygonMesh* m, std::string filename) throw(ExceptionMessage){
	std::string filenameNode = FileUtils::getFileNameWithoutExtension(filename)+".node";
	std::string filenameEle = FileUtils::getFileNameWithoutExtension(filename)+".ele";
	exportVertices(m,filenameNode);
	std::ofstream outputFile(filenameEle.c_str());
	std::vector<vis::Polygon*>& polygons = m->getPolygons();
	int numberOfTriangles = 0;
	for( vis::Polygon* polygon : polygons )
		numberOfTriangles += (polygon->getVertices().size()-2);
	outputFile << numberOfTriangles << " 3 0" << std::endl;

	int currentTriangleId = 1;
	for( vis::Polygon* polygon : polygons){
		std::vector<vis::Vertex*>& currentPolygonVertices = polygon->getVertices();
		for( decltype(currentPolygonVertices.size()) j = 1; j < currentPolygonVertices.size() - 1; j++ ) {
			outputFile << currentTriangleId++ << " ";
			outputFile << currentPolygonVertices[0]->getId() << " ";
			outputFile << currentPolygonVertices[j]->getId() << " ";
			outputFile << currentPolygonVertices[j+1]->getId() << std::endl;
		}
	}
	outputFile << "#Exported with Vis from a PolygonMesh";
	outputFile.close();
	return true;
}

bool ModelExportEleNode::exportModel(PolyhedronMesh* m, std::string filename) throw(ExceptionMessage){
	std::string filenameNode = FileUtils::getFileNameWithoutExtension(filename)+".node";
	std::string filenameEle = FileUtils::getFileNameWithoutExtension(filename)+".ele";
	exportVertices(m,filenameNode);
	std::ofstream outputFile(filenameEle.c_str());
	std::vector<vis::Polyhedron*>& polyhedrons = m->getPolyhedrons();
	int numberOfTetrahedrons = 0;
	for( vis::Polyhedron* polyhedron : polyhedrons)
		if(polyhedron->getPolyhedronPolygons().size()==4)
			numberOfTetrahedrons++;
	outputFile << numberOfTetrahedrons << " 4 0" << std::endl;
	for( vis::Polyhedron* polyhedron : polyhedrons ){
		if(polyhedron->getPolyhedronPolygons().size()!=4)
			continue;
		std::vector<vis::Vertex*> currentTetraVertices;
		PolyhedronUtils::getPolyhedronVertices(polyhedron,currentTetraVertices);
		if(currentTetraVertices.size()!=4)
			continue;
		outputFile << polyhedron->getId() << " ";
		outputFile << currentTetraVertices[0]->getId() << " ";
		outputFile << currentTetraVertices[1]->getId() << " ";
		outputFile << currentTetraVertices[2]->getId() << " ";
		outputFile << currentTetraVertices[3]->getId() << std::endl;
	}
	outputFile << "#Exported with Vis from a PolygonMesh";
	outputFile.close();
	return true;
}
void ModelExportEleNode::exportVertices(VertexCloud* m,std::string filename)throw(ExceptionMessage) {
	std::ofstream outputFile(filename.c_str());
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	outputFile << vertices.size() << " " << ((m->is2D())?2:3) << " 0 0" << std::endl;
	for( vis::Vertex* vertex : vertices){
		outputFile << vertex->getId() << " ";
		outputFile << vertex->getCoords().x << " ";
		if(m->is2D())
			outputFile << vertex->getCoords().y << std::endl;
		else{
			outputFile << vertex->getCoords().y << " ";
			outputFile << vertex->getCoords().z << std::endl;
		}
	}
	outputFile << "#Exported with vis";
	outputFile.close();
}

#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportEleNode);
