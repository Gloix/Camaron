#ifndef MODELLOADINGTS_H
#define MODELLOADINGTS_H
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/chararrayscanner.h"
class ModelLoadingTs:public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingTs)
	protected:
	private:
		char* fileBuffer;
		long fileSize;
		PolyhedronMesh* readHeader(std::string filename);
		bool readBody( PolyhedronMesh* );
		bool readPolyhedrons( PolyhedronMesh* );
		bool readVertices( PolyhedronMesh* );
		CharArrayScanner scanner;
};

#endif // MODELLOADINGTS_H
