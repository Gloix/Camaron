#ifndef MODELLOADINGOFFLW_H
#define MODELLOADINGOFFLW_H

#include "Utils/chararrayscanner.h"
#include "ModelLoading/ModelLoadingStrategy.h"
class LightWeightPolygonMesh;
class LightWeightVertexCloud;
class Model;

class ModelLoadingOffLW: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingOffLW)
	protected:
	private:
		char* fileBuffer;
		long fileSize;
		bool readHeader(LightWeightPolygonMesh*);
		bool readBody( LightWeightPolygonMesh* );
		bool readPolygons( LightWeightPolygonMesh* );
		bool readVertices( LightWeightVertexCloud * );
		CharArrayScanner scanner;
};

#endif // MODELLOADINGOFFLW_H
