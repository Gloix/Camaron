#ifndef MODELLOADINGELENODELW_H
#define MODELLOADINGELENODELW_H
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/chararrayscanner.h"
#include <unordered_map>
#include <memory>
class ScalarFieldDef;
class ModelLoadingEleNodeLW: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingEleNodeLW)
	protected:
	private:
		char* fileBufferNode;
		long fileSizeNode;
		char* fileBufferEle;
		long fileSizeEle;
		void readHeaderNode();
		void readHeaderEle();
		bool readBody( LightWeightPolygonMesh* );
		bool readPolygons( LightWeightPolygonMesh* );
		bool readPolyhedrons( LightWeightPolyhedronMesh* );
		bool readVertices( LightWeightPolygonMesh* );

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

};

#endif // MODELLOADINGELENODELW_H
