#ifndef MODELLOADINGTRI_H
#define MODELLOADINGTRI_H

#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/chararrayscanner.h"
class ModelLoadingTri:public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingTri)
	protected:
	private:
		char* fileBuffer;
		long fileSize;
		bool readVertices( PolygonMesh* );
		bool readPolygons( PolygonMesh* );
		bool readBody( PolygonMesh* );
		CharArrayScanner scanner;
};

#endif // MODELLOADINGTRI_H
