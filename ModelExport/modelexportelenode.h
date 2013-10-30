#ifndef MODELEXPORTELENODE_H
#define MODELEXPORTELENODE_H

#include "ModelExport/modelexportstrategy.h"
class ModelExportEleNode: public ModelExportStrategy
{
	public:
		ModelExportEleNode();
		virtual ~ModelExportEleNode();
		virtual bool exportModel(PolygonMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportModel(PolyhedronMesh*, std::string filename) throw(ExceptionMessage);
	private:
		void exportVertices(VertexCloud*,std::string filename) throw(ExceptionMessage);
};

#endif // MODELEXPORTELENODE_H
