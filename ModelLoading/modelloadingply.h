#ifndef MODELLOADINGPLY_H
#define MODELLOADINGPLY_H
#include "Utils/chararrayscanner.h"
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Model/Element/Vertex.h"
class PolygonMesh;
class VertexCloud;
class Model;
struct VScalarDef;

class ModelLoadingPly: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingPly)
	protected:
	private:
		char* fileBuffer;
		long fileSize;
		bool readHeader(PolygonMesh*);
		bool readBody( PolygonMesh* );
		bool readPolygons( PolygonMesh* );
		bool readVertices( VertexCloud * );
		bool readAdditionalEdges( VertexCloud * );
		vis::Vertex* readVertex(int, float&, float&, float&);
		bool readPolygonsBinary( PolygonMesh* );
		bool readVerticesBinary( VertexCloud * );
		CharArrayScanner scanner;
		int numberOfBytesInVertexPropertiesToIgnore;
		std::vector<VScalarDef*> vertexProperties;
		bool isBinary;
		bool bigEndian;
};

#endif // MODELLOADINGPLY_H
