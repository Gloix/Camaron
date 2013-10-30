#ifndef MODELLOADINGPLY_H
#define MODELLOADINGPLY_H
#include "Utils/chararrayscanner.h"
#include "ModelLoading/ModelLoadingStrategy.h"
class PolygonMesh;
class VertexCloud;
class Model;

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
		bool readPolygonsBinary( PolygonMesh* );
		bool readVerticesBinary( VertexCloud * );
		CharArrayScanner scanner;
		int numberOfBytesInVertexPropertiesToIgnore;
		bool isBinary;
		bool bigEndian;
};

#endif // MODELLOADINGPLY_H
