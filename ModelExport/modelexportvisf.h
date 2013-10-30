#ifndef MODELEXPORTVISF_H
#define MODELEXPORTVISF_H
#include "ModelExport/modelexportstrategy.h"
#include <unordered_map>
#include <fstream>
namespace vis {
	class Element;
}
class ModelExportVisF: public ModelExportStrategy
{
	public:
		ModelExportVisF();
		virtual ~ModelExportVisF();
		virtual bool exportModel(VertexCloud*, std::string filename) throw(ExceptionMessage);
		virtual bool exportModel(PolygonMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportModel(PolyhedronMesh*, std::string filename) throw(ExceptionMessage);
		virtual bool exportSelection(Selection*, std::string filename) throw(ExceptionMessage);
	private:
		bool exportSelectedPolyhedrons(std::unordered_map<int,vis::Element*>& selectedElements,
									std::string filename) throw(ExceptionMessage);
		bool exportSelectedPolygons(std::unordered_map<int,vis::Element*>& selectedElements,
									std::string filename) throw(ExceptionMessage);

		bool exportVertices(VertexCloud*, std::ofstream& outputFile);
		bool exportPolygons(PolygonMesh*m, std::ofstream& outputFile);
		bool exportPolyhedrons(PolyhedronMesh*, std::ofstream& outputFile);

		template<class T> inline void writeData(std::ofstream&, T val);
};
template<class T>
void ModelExportVisF::writeData(std::ofstream& file, T val){
	file.write((const char*)&val,sizeof(T));
}

#endif // MODELEXPORTVISF_H
