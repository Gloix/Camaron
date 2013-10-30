#ifndef MODELLOADINGM3D_H
#define MODELLOADINGM3D_H

#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/chararrayscanner.h"
#include "Utils/hashingtree.h"
namespace vis{
class Polygon;
class Vertex;
}
class ModelLoadingM3D: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingM3D)
		private:
			char* fileBuffer;
		long fileSize;
		bool readBody( PolyhedronMesh* );
		bool readPolyhedrons( PolyhedronMesh* );
		bool readVertices( PolyhedronMesh* );
		bool moveToElements(PolyhedronMesh* );

		//read Elements
		void readM3DPolyhedron(int nVertices,
							   std::vector<std::vector<int> >& verticesIndicesPerFace,
							   PolyhedronMesh* pol,
							   vis::ut::HashingTree<int,vis::Polygon>& hashingTree);
		void readTetrahedron(PolyhedronMesh*,vis::ut::HashingTree<int,vis::Polygon>&);
		void readPyramid(PolyhedronMesh*,vis::ut::HashingTree<int,vis::Polygon>&);
		void readPrism(PolyhedronMesh*,vis::ut::HashingTree<int,vis::Polygon>&);
		void readHexahedron(PolyhedronMesh*,vis::ut::HashingTree<int,vis::Polygon>&);
		CharArrayScanner scanner;
		vis::Polygon* createPolygon(std::vector<int>& indices,
									std::vector<vis::Vertex*>& vertices,
									int polygonIndex,
									vis::ut::HashingTree<int,vis::Polygon>& hashingTree);
};


#endif // MODELLOADINGM3D_H
