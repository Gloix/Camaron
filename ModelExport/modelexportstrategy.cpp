#include "modelexportstrategy.h"
#include "Model/Model.h"
#include "Utils/fileutils.h"
ModelExportStrategy::ModelExportStrategy(std::string n, std::string e):
	name(n), extension(e)
{
}
ModelExportStrategy::~ModelExportStrategy()
{
}
bool ModelExportStrategy::exportModel(VertexCloud*, std::string filename) throw(ExceptionMessage){
	throw ExceptionMessage("Can't export Vertex Cloud Mesh with extension: " +
						   FileUtils::getFileNameExtension(filename));
}

bool ModelExportStrategy::exportModel(PolygonMesh*, std::string filename) throw(ExceptionMessage){
	throw ExceptionMessage("Can't export Polygon Mesh with extension: " +
						   FileUtils::getFileNameExtension(filename));
}

bool ModelExportStrategy::exportModel(PolyhedronMesh*, std::string filename) throw(ExceptionMessage){
	throw ExceptionMessage("Can't export Polyhedron Mesh with extension: " +
						   FileUtils::getFileNameExtension(filename));
}
bool ModelExportStrategy::exportSelection(Selection*, std::string filename) throw(ExceptionMessage){
	throw ExceptionMessage("Can't export selection with extension: " +
						   FileUtils::getFileNameExtension(filename));
}

bool ModelExportStrategy::exportModel(Model* m, std::string filename){
	return m->exportModelDD(this,filename);
}
std::string ModelExportStrategy::getName(){
	return name;
}

std::string ModelExportStrategy::getExtension(){
	return extension;
}
