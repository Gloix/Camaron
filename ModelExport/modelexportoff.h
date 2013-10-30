#ifndef MODELEXPORTOFF_H
#define MODELEXPORTOFF_H
#include "ModelExport/modelexportstrategy.h"

class ModelExportOFF: public ModelExportStrategy
{
	public:
		ModelExportOFF();
		virtual ~ModelExportOFF();
		virtual bool exportModel(PolygonMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportModel(PolyhedronMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportSelection(Selection*, std::string filename) throw(ExceptionMessage);
};

#endif // MODELEXPORTOFF_H
