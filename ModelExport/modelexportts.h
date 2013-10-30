#ifndef MODELEXPORTTS_H
#define MODELEXPORTTS_H

#include "ModelExport/modelexportstrategy.h"
class ModelExportTS: public ModelExportStrategy
{
	public:
		ModelExportTS();
		virtual ~ModelExportTS();
		virtual bool exportModel(PolyhedronMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportSelection(Selection*, std::string filename) throw(ExceptionMessage);
};

#endif // MODELEXPORTTS_H
