#include "modelexportvisf.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "SelectionStrategies/Selection.h"
ModelExportVisF::ModelExportVisF():
	ModelExportStrategy("VisF","visf")
{
}
ModelExportVisF::~ModelExportVisF(){

}
bool ModelExportVisF::exportModel(PolygonMesh* m, std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(), std::ios::trunc | std::ios::binary);
	int polygonType = vis::CONSTANTS::POLYGON_MESH;
	writeData(outputFile,polygonType);
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	std::vector<vis::Polygon*>& polygons = m->getPolygons();
	writeData(outputFile,(int)vertices.size());

	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
		writeData(outputFile,vertices[i]->getCoords().x);
		writeData(outputFile,vertices[i]->getCoords().y);
		writeData(outputFile,vertices[i]->getCoords().z);
	}
	writeData(outputFile,(int)polygons.size());
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Vertex*>& currentPolygonVertices = polygons[i]->getVertices();
		writeData(outputFile,(int)currentPolygonVertices.size());
		for(std::vector<vis::Vertex*>::size_type j = 0;j<currentPolygonVertices.size();j++)
			writeData(outputFile,currentPolygonVertices[j]->getPos());
	}
	//saving neighbors
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Polygon*>& neighbors = polygons[i]->getNeighborPolygons();
		writeData(outputFile,(int)neighbors.size());
		for(std::vector<vis::Polygon*>::size_type j = 0;j<neighbors.size();j++)
			writeData(outputFile,neighbors[j]->getId());
	}
	outputFile.close();
	return true;
}

bool ModelExportVisF::exportModel(PolyhedronMesh* m, std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(), std::ios::trunc | std::ios::binary);
	int polyhedronType = vis::CONSTANTS::POLYHEDRON_MESH;
	writeData(outputFile,polyhedronType);
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	std::vector<vis::Polygon*>& polygons = m->getPolygons();
	std::vector<vis::Polyhedron*>& polyhedrons = m->getPolyhedrons();
	writeData(outputFile,(int)vertices.size());
	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
		writeData(outputFile,vertices[i]->getCoords().x);
		writeData(outputFile,vertices[i]->getCoords().y);
		writeData(outputFile,vertices[i]->getCoords().z);
	}
	writeData(outputFile,(int)polygons.size());
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Vertex*>& currentPolygonVertices = polygons[i]->getVertices();
		writeData(outputFile,(int)currentPolygonVertices.size());
		for(std::vector<vis::Vertex*>::size_type j = 0;j<currentPolygonVertices.size();j++)
			writeData(outputFile,currentPolygonVertices[j]->getPos());
	}
	//saving neighbors
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Polygon*>& neighbors = polygons[i]->getNeighborPolygons();
		writeData(outputFile,(int)neighbors.size());
		for(std::vector<vis::Polygon*>::size_type j = 0;j<neighbors.size();j++)
			writeData(outputFile,neighbors[j]->getId());
	}
	writeData(outputFile,(int)polyhedrons.size());
	for(std::vector<vis::Polyhedron*>::size_type i = 0;i<polyhedrons.size();i++){
		std::vector<vis::Polygon*>& currentPolygons = polyhedrons[i]->getPolyhedronPolygons();
		writeData(outputFile,(int)currentPolygons.size());
		for(std::vector<vis::Polygon*>::size_type j = 0;j<currentPolygons.size();j++)
			writeData(outputFile,currentPolygons[j]->getId());
	}

	outputFile.close();
	return true;
}
bool ModelExportVisF::exportSelectedPolygons(std::unordered_map<int,vis::Element*>& selectedElements,
											 std::string filename) throw(ExceptionMessage){

	std::ofstream outputFile(filename.c_str());
	outputFile << "OFF" << std::endl;
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
	outputFile << exportedVertices.size() << " " << selectedElements.size() << " 0" << std::endl;
	typedef std::unordered_map<int,vis::Vertex*>::const_iterator it_vertex_type;
	int vertexFilePos = 0;
	for(it_vertex_type it = exportedVertices.begin();it!=exportedVertices.end();it++){
		vis::Vertex* current = it->second;
		outputFile << current->getCoords().x << " ";
		outputFile << current->getCoords().y << " ";
		outputFile << current->getCoords().z << std::endl;
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
		outputFile << std::endl;
	}
	vertexIdVsVertexExportedPos.clear();
	outputFile << "#Exported with Vis from a Polygon Selection";
	outputFile.close();
	return true;
}

bool ModelExportVisF::exportSelectedPolyhedrons(std::unordered_map<int,vis::Element*>& selectedElements,
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

bool ModelExportVisF::exportSelection(Selection* se, std::string filename) throw(ExceptionMessage){
	if(se->getSelectionType() == vis::CONSTANTS::POLYGON)
		return exportSelectedPolygons(se->getSelectedElements(),filename);
	else if(se->getSelectionType() == vis::CONSTANTS::POLYHEDRON)
		return exportSelectedPolyhedrons(se->getSelectedElements(),filename);
	return false;
}

#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportVisF);
