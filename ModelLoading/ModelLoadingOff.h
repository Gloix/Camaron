#ifndef MODELLOADINGOFF_H
#define MODELLOADINGOFF_H
#include "Utils/chararrayscanner.h"
#include "ModelLoading/ModelLoadingStrategy.h"
class PolygonMesh;
class VertexCloud;
class Model;

class ModelLoadingOff: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingOff)
	protected:
	private:
		char* fileBuffer;
		long fileSize;
		bool readHeader(PolygonMesh*);
		bool readBody( PolygonMesh* );
		bool readPolygons( PolygonMesh* );
		bool readVertices( VertexCloud * );
		CharArrayScanner scanner;
};

#endif // MODELLOADINGOFF_H
