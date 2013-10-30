#include "modelexportoff.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Common/Constants.h"
#include "SelectionStrategies/Selection.h"
#include <iostream>
#include <fstream>
ModelExportOFF::ModelExportOFF():
	ModelExportStrategy("OFF","off")
{
}
ModelExportOFF::~ModelExportOFF(){

}
bool ModelExportOFF::exportModel(PolygonMesh* m, std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(),std::ios::binary);
	outputFile << "OFF" << "\n";
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	std::vector<vis::Polygon*>& polygons = m->getPolygons();
	outputFile << vertices.size() << " " << polygons.size() << " 0" << "\n";

	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
		outputFile << vertices[i]->getCoords().x << " ";
		outputFile << vertices[i]->getCoords().y << " ";
		outputFile << vertices[i]->getCoords().z << "\n";
	}
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Vertex*>& currentPolygonVertices = polygons[i]->getVertices();
		outputFile << currentPolygonVertices.size() << " ";
		for(std::vector<vis::Vertex*>::size_type j = 0;j<currentPolygonVertices.size();j++){
			outputFile << currentPolygonVertices[j]->getPos() << " ";

		}
		outputFile << "\n";
	}
	outputFile << "#Exported with Vis from a PolygonMesh";
	outputFile.close();
	return true;
}

bool ModelExportOFF::exportModel(PolyhedronMesh* m, std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(),std::ios::binary);
	outputFile << "OFF" << "\n";
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	std::vector<vis::Polygon*>& polygons = m->getPolygons();
	outputFile << vertices.size() << " " << polygons.size() << " 0" << "\n";
	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
		outputFile << vertices[i]->getCoords().x << " ";
		outputFile << vertices[i]->getCoords().y << " ";
		outputFile << vertices[i]->getCoords().z << "\n";
	}
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Vertex*>& currentPolygonVertices = polygons[i]->getVertices();
		outputFile << currentPolygonVertices.size() << " ";
		for(std::vector<vis::Vertex*>::size_type j = 0;j<currentPolygonVertices.size();j++){
			outputFile << currentPolygonVertices[j]->getPos() << " ";

		}
		outputFile << "\n";
	}
	outputFile << "#Exported with Vis from a PolyhedronMesh";
	outputFile.close();
	return true;
}
bool exportSelectedPolygons(std::unordered_map<int,vis::Element*>& selectedElements,
							std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str());
	outputFile << "OFF" << "\n";
	std::unordered_map<int,vis::Vertex*> exportedVertices;
	std::unordered_map<int,int> vertexIdVsVertexExportedPos;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polygon* polygon = (vis::Polygon*)it->second;
		std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		for(std::vector<vis::Vertex*>::size_type v = 0;
			v<polygonVertices.size();v++)
			exportedVertices[polygonVertices[v]->getId()] = polygonVertices[v];
	}
	outputFile << exportedVertices.size() << " " << selectedElements.size() << " 0" << "\n";
	typedef std::unordered_map<int,vis::Vertex*>::const_iterator it_vertex_type;
	int vertexFilePos = 0;
	for(it_vertex_type it = exportedVertices.begin();it!=exportedVertices.end();it++){
		vis::Vertex* current = it->second;
		outputFile << current->getCoords().x << " ";
		outputFile << current->getCoords().y << " ";
		outputFile << current->getCoords().z << "\n";
		vertexIdVsVertexExportedPos[current->getId()] = vertexFilePos;
		vertexFilePos++;
	}
	exportedVertices.clear();
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polygon* polygon = (vis::Polygon*)it->second;
		std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		outputFile << polygonVertices.size();
		for(std::vector<vis::Vertex*>::size_type v = 0;
			v<polygonVertices.size();v++)
			outputFile << " " << vertexIdVsVertexExportedPos[polygonVertices[v]->getId()];
		outputFile << "\n";
	}
	vertexIdVsVertexExportedPos.clear();
	outputFile << "#Exported with Vis from a Polygon Selection";
	outputFile.close();
	return true;
}

bool exportSelectedPolyhedrons(std::unordered_map<int,vis::Element*>& selectedElements,
							std::string filename) throw(ExceptionMessage){
	std::unordered_map<int,vis::Element*> selectedPolygons;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polyhedron* polyhedron = (vis::Polyhedron*)it->second;
		std::vector<vis::Polygon*>& polyhedronPolygons = polyhedron->getPolyhedronPolygons();
		for(std::vector<vis::Polygon*>::size_type v = 0;
			v<polyhedronPolygons.size();v++){
			selectedPolygons[polyhedronPolygons[v]->getId()] = polyhedronPolygons[v];
		}
	}
	return exportSelectedPolygons(selectedPolygons,filename);
}

bool ModelExportOFF::exportSelection(Selection* se, std::string filename) throw(ExceptionMessage){
	if(se->getSelectionType() == vis::CONSTANTS::POLYGON)
		return exportSelectedPolygons(se->getSelectedElements(),filename);
	else if(se->getSelectionType() == vis::CONSTANTS::POLYHEDRON)
		return exportSelectedPolyhedrons(se->getSelectedElements(),filename);
	return false;
}

#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportOFF);
