#ifndef PROPERTYFIELDLOADINGPLY_H
#define PROPERTYFIELDLOADINGPLY_H
#include "Utils/chararrayscanner.h"
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "PropertyFieldLoading/propertyfielddefvisitor.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/Element/Vertex.h"
#include "Model/modelvisitor.h"
#include "Utils/fileutils.h"
#include <vector>
#include <memory>
#include <sstream>
class ScalarFieldDef;
class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class Model;

class PropertyFieldLoadingPly: public PropertyFieldLoadingStrategy, public ModelVisitor
{
	public:
		PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(PropertyFieldLoadingPly)
		void visit(VertexCloud* model);
		void visit(PolygonMesh* model);
		void visit(PolyhedronMesh* model);

	private:
		bool readModelProperties( std::string, VertexCloud* , std::vector<std::shared_ptr<PropertyFieldDef>> );
		std::vector<float> readVertexProperties(std::ifstream&, int, std::vector<std::shared_ptr<PropertyFieldDef>>);

		std::string filename;
		std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties;
};

#endif // PROPERTYFIELDLOADINGPLY_H
