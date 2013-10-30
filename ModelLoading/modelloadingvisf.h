#ifndef MODELLOADINGVISF_H
#define MODELLOADINGVISF_H
#include "ModelLoading/ModelLoadingStrategy.h"
#include <fstream>
class VertexCloud;
class ModelLoadingVisF: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingVisF)
	protected:
	private:
		bool readHeader(PolygonMesh*);
		bool readBody( PolygonMesh* );
		bool readPolyhedrons( PolyhedronMesh* );
		bool readPolygons( PolygonMesh* );
		bool readVertices( VertexCloud* );
		std::ifstream file;
};

#endif // MODELLOADINGVISF_H
