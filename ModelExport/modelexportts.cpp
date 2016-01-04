#include "modelexportts.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Vertex.h"
#include "Utils/polyhedronutils.h"
#include <iostream>
#include <fstream>
ModelExportTS::ModelExportTS():
	ModelExportStrategy("TS","ts")
{
}
ModelExportTS::~ModelExportTS(){

}

bool ModelExportTS::exportModel(PolyhedronMesh* m,
								std::string filename) throw(ExceptionMessage){
	std::ofstream outputFile(filename.c_str(),std::ios::binary);
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	std::vector<vis::Polyhedron*>& polyhedrons = m->getPolyhedrons();
	outputFile << vertices.size() << " " << polyhedrons.size() << std::endl;
	for( vis::Vertex* vertex : vertices){
		outputFile << vertex->getCoords().x << " ";
		outputFile << vertex->getCoords().y << " ";
		outputFile << vertex->getCoords().z << " ";
		outputFile << "1.0" << std::endl;
	}
	for( vis::Polyhedron* polyhedron : polyhedrons ){
		std::vector<vis::Vertex*> currentTetraVertices;
		PolyhedronUtils::getPolyhedronVertices(polyhedron,currentTetraVertices);
		if(currentTetraVertices.size()!=4)
			continue;
		outputFile << currentTetraVertices[0]->getPos() << " ";
		outputFile << currentTetraVertices[1]->getPos() << " ";
		outputFile << currentTetraVertices[2]->getPos() << " ";
		outputFile << currentTetraVertices[3]->getPos() << std::endl;
	}
	outputFile << "#Exported with Vis from a PolyhedronMesh";
	outputFile.close();
	return true;
}

bool ModelExportTS::exportSelection(Selection*,
									std::string filename) throw(ExceptionMessage){

	return false;
}
#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportTS);
