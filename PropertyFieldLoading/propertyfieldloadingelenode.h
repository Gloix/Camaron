#ifndef PROPERTYFIELDLOADINGELENODE_H
#define PROPERTYFIELDLOADINGELENODE_H
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "Model/modelvisitor.h"
#include "Utils/chararrayscanner.h"
#include <unordered_map>

class PropertyFieldLoadingEleNode: public PropertyFieldLoadingStrategy, public ModelVisitor
{
	public:
		PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(PropertyFieldLoadingEleNode)
		void visit(VertexCloud* model);
		void visit(PolygonMesh* model);
		void visit(PolyhedronMesh* model);
	protected:
	private:
		void readHeaderNode(std::ifstream& file, std::vector<std::shared_ptr<PropertyFieldDef>>& vertexProperties);
		bool readVertices( Model* );
		bool readModelProperties( std::string filename, VertexCloud* vcloud, std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties);

		std::string filename;
		std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties;
		//Node header
		int numberOfNodes;
		int dimensions;
		int numberOfAttributesPerNode;
		int numberOfBoundaryMarkers;

};

#endif // PROPERTYFIELDLOADINGELENODE_H
