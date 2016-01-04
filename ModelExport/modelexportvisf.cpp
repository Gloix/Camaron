#include "modelexportvisf.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "SelectionStrategies/Selection.h"
#include "Utils/endianess.h"
ModelExportVisF::ModelExportVisF():
	ModelExportStrategy("VisF","visf")
{
}
ModelExportVisF::~ModelExportVisF(){

}

bool ModelExportVisF::exportModel(VertexCloud* m, std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(), std::ios::trunc | std::ios::binary);
	int vertexCloudType = vis::CONSTANTS::VERTEX_CLOUD;
	unsigned char endianness = Endianess::findEndianness();
	writeData(outputFile,endianness);
	writeData(outputFile,vertexCloudType);

	ModelExportVisF::exportVertices(m,outputFile);

	outputFile.close();
	return true;
}

bool ModelExportVisF::exportModel(PolygonMesh* m, std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(), std::ios::trunc | std::ios::binary);
	int polygonType = vis::CONSTANTS::POLYGON_MESH;
	unsigned char endianness = Endianess::findEndianness();
	writeData(outputFile,endianness);
	writeData(outputFile,polygonType);

	ModelExportVisF::exportVertices(m,outputFile);
	ModelExportVisF::exportPolygons(m,outputFile);

	outputFile.close();
	return true;
}

bool ModelExportVisF::exportModel(PolyhedronMesh* m, std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(), std::ios::trunc | std::ios::binary);
	unsigned char endianness = Endianess::findEndianness();
	writeData(outputFile,endianness);
	int polyhedronType = vis::CONSTANTS::POLYHEDRON_MESH;
	writeData(outputFile,polyhedronType);

	ModelExportVisF::exportVertices(m,outputFile);
	ModelExportVisF::exportPolygons(m,outputFile);
	ModelExportVisF::exportPolyhedrons(m,outputFile);

	outputFile.close();
	return true;
}


bool ModelExportVisF::exportVertices(VertexCloud* m, std::ofstream& outputFile){
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	writeData(outputFile,(int)vertices.size());
	for( vis::Vertex* vertex : vertices ){
		writeData(outputFile,vertex->getCoords().x);
		writeData(outputFile,vertex->getCoords().y);
		writeData(outputFile,vertex->getCoords().z);
	}
	return true;
}

bool ModelExportVisF::exportPolygons(PolygonMesh * m, std::ofstream& outputFile){
	std::vector<vis::Polygon*>& polygons = m->getPolygons();
	writeData(outputFile,(int)polygons.size());
	for(vis::Polygon* polygon : polygons){
		std::vector<vis::Vertex*>& currentPolygonVertices = polygon->getVertices();
		writeData(outputFile,(int)currentPolygonVertices.size());
		for(vis::Vertex* vertex : currentPolygonVertices)
			writeData(outputFile,vertex->getPos());
	}
	//saving neighbors
	writeData(outputFile,1); //exported with neighborhood info
	for( vis::Polygon* polygon : polygons){
		std::vector<vis::Polygon*>& neighbors = polygon->getNeighborPolygons();
		writeData(outputFile,(int)neighbors.size());
		for( vis::Polygon* neighbor : neighbors)
			writeData(outputFile,neighbor->getId());
	}
	return true;
}

bool ModelExportVisF::exportPolyhedrons(PolyhedronMesh * m, std::ofstream& outputFile){
	std::vector<vis::Polyhedron*>& polyhedrons = m->getPolyhedrons();
	writeData(outputFile,(int)polyhedrons.size());
	for( vis::Polyhedron* polyhedron : polyhedrons){
		std::vector<vis::Polygon*>& currentPolygons = polyhedron->getPolyhedronPolygons();
		writeData(outputFile,(int)currentPolygons.size());
		for(vis::Polygon* polygon : currentPolygons)
			writeData(outputFile,polygon->getId());
	}
	return true;
}


bool ModelExportVisF::exportSelectedPolygons(std::unordered_map<int,vis::Element*>& selectedElements,
											 std::string filename) throw(ExceptionMessage){
	//TODO: ESTO NO ESTA FUNCIONANDO
	std::ofstream outputFile(filename.c_str());
	outputFile << "OFF" << std::endl;
	std::unordered_map<int,vis::Vertex*> exportedVertices;
	std::unordered_map<int,int> vertexIdVsVertexExportedPos;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polygon* polygon = (vis::Polygon*)it->second;
		std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		for( vis::Vertex* vertex : polygonVertices)
			exportedVertices[vertex->getId()] = vertex;
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
		for(vis::Vertex* vertex : polygonVertices)
			outputFile << " " << vertexIdVsVertexExportedPos[vertex->getId()];
		outputFile << std::endl;
	}
	vertexIdVsVertexExportedPos.clear();
	outputFile << "#Exported with Vis from a Polygon Selection";
	outputFile.close();
	return true;
}


bool ModelExportVisF::exportSelectedPolyhedrons(std::unordered_map<int,vis::Element*>& selectedElements,
												std::string filename) throw(ExceptionMessage){
	//TODO: ESTO NO ESTA FUNCIONANDO
	std::unordered_map<int,vis::Element*> selectedPolygons;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polyhedron* polyhedron = (vis::Polyhedron*)it->second;
		std::vector<vis::Polygon*>& polyhedronPolygons = polyhedron->getPolyhedronPolygons();
		for(vis::Polygon* polygon : polyhedronPolygons){
			selectedPolygons[polygon->getId()] = polygon;
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
