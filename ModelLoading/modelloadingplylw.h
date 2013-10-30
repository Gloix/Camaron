#ifndef MODELLOADINGPLYLW_H
#define MODELLOADINGPLYLW_H
#include "Utils/chararrayscanner.h"
#include "ModelLoading/ModelLoadingStrategy.h"

class ModelLoadingPlyLW: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingPlyLW)
	protected:
	private:
		char* fileBuffer;
		long fileSize;
		bool readHeader(LightWeightPolygonMesh*);
		bool readBody( LightWeightPolygonMesh* );
		bool readPolygons( LightWeightPolygonMesh* );
		bool readVertices( LightWeightVertexCloud * );
		bool readPolygonsBinary( LightWeightPolygonMesh* );
		bool readVerticesBinary( LightWeightVertexCloud * );
		CharArrayScanner scanner;
		int numberOfBytesInVertexPropertiesToIgnore;
		bool isBinary;
		bool bigEndian;
};

#endif // MODELLOADINGPLYLW_H
