#ifndef MODELEXPORTSTRATEGY_H
#define MODELEXPORTSTRATEGY_H
#include <string>
#include "Exceptions/exceptionmessage.h"
class Model;
class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class Selection;
class ModelExportStrategy
{
	public:
		ModelExportStrategy(std::string n, std::string e);
		virtual ~ModelExportStrategy();
		virtual bool exportModel(VertexCloud*, std::string filename) throw(ExceptionMessage);
		virtual bool exportModel(PolygonMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportModel(PolyhedronMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportSelection(Selection*, std::string filename) throw(ExceptionMessage);
		virtual bool exportModel(Model* m, std::string filename);

		std::string getName();
		std::string getExtension();

	private:
		std::string name;
		std::string extension;
};

#endif // MODELEXPORTSTRATEGY_H
