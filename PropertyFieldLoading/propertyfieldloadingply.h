#ifndef PROPERTYFIELDLOADINGPLY_H
#define PROPERTYFIELDLOADINGPLY_H
#include "Utils/chararrayscanner.h"
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "Model/Element/Vertex.h"
class PolygonMesh;
class VertexCloud;
class Model;
struct VScalarDef;

class PropertyFieldLoadingPly: public PropertyFieldLoadingStrategy
{
	public:
		PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(PropertyFieldLoadingPly)
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

#endif // PROPERTYFIELDLOADINGPLY_H
