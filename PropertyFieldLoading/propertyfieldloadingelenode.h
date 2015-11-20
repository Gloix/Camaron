#ifndef PROPERTYFIELDLOADINGELENODE_H
#define PROPERTYFIELDLOADINGELENODE_H
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "Utils/chararrayscanner.h"
#include <unordered_map>
struct VScalarDef;
class PropertyFieldLoadingEleNode: public PropertyFieldLoadingStrategy
{
	public:
		PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(PropertyFieldLoadingEleNode)
	protected:
	private:
		char* fileBufferNode;
		long fileSizeNode;
		char* fileBufferEle;
		long fileSizeEle;
		void readHeaderNode();
		void readHeaderEle();
		bool readBody( PolygonMesh* );
		bool readPolygons( PolygonMesh* );
		bool readPolyhedrons( PolyhedronMesh* );
		bool readVertices( PolygonMesh* );

		CharArrayScanner scanner;

		bool isPolygonMesh;
		//Ele header
		int numberOfElements;
		int numberOfAttributesPerElement;
		int numberOfNodesPerElement;
		//Node header
		int numberOfNodes;
		int dimensions;
		int numberOfAttributesPerNode;
		int numberOfBoundaryMarkers;
		std::unordered_map<int,int> indexVsPosition;
		std::vector<VScalarDef*> vertexProperties;

};

#endif // PROPERTYFIELDLOADINGELENODE_H
