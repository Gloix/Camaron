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
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++)
		numberOfTriangles += (polygons[i]->getVertices().size()-2);
	outputFile << numberOfTriangles << " 3 0" << std::endl;

	int currentTriangleId = 1;
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Vertex*>& currentPolygonVertices = polygons[i]->getVertices();
		for( std::vector<vis::Vertex*>::size_type j = 1; j < currentPolygonVertices.size() - 1; j++ ) {
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
	for(std::vector<vis::Polyhedron*>::size_type i = 0;i<polyhedrons.size();i++)
		if(polyhedrons[i]->getPolyhedronPolygons().size()==4)
			numberOfTetrahedrons++;
	outputFile << numberOfTetrahedrons << " 4 0" << std::endl;
	for(std::vector<vis::Polyhedron*>::size_type i = 0;i<polyhedrons.size();i++){
		if(polyhedrons[i]->getPolyhedronPolygons().size()!=4)
			continue;
		std::vector<vis::Vertex*> currentTetraVertices;
		PolyhedronUtils::getPolyhedronVertices(polyhedrons[i],currentTetraVertices);
		if(currentTetraVertices.size()!=4)
			continue;
		outputFile << polyhedrons[i]->getId() << " ";
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
	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
		outputFile << vertices[i]->getId() << " ";
		outputFile << vertices[i]->getCoords().x << " ";
		if(m->is2D())
			outputFile << vertices[i]->getCoords().y << std::endl;
		else{
			outputFile << vertices[i]->getCoords().y << " ";
			outputFile << vertices[i]->getCoords().z << std::endl;
		}
	}
	outputFile << "#Exported with vis";
	outputFile.close();
}

#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportEleNode);
