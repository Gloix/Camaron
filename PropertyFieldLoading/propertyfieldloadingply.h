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
struct VScalarDef;

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

class PropertyReader : public PropertyFieldDefVisitor {
	std::ifstream* file;
	vis::Vertex* vertex;
	std::string line;
	std::unique_ptr<std::istringstream> iss;
	int currentIndex;
	public:
		PropertyReader(std::ifstream* file) : file(file){}

		void setup(vis::Vertex* vertex, int index) {
			FileUtils::safeGetline(*file, line);
			iss.reset(new std::istringstream(line));
			this->vertex = vertex;
			currentIndex = index;
		}

		void visit(std::shared_ptr<ScalarFieldDef> scalarFieldDef)
		{
			//std::vector<float> properties;
			//std::map<int, std::shared_ptr<ScalarFieldDef>> propertyIds;
			//for(std::shared_ptr<PropertyFieldDef> prop : scalarProperties) {
			//	propertyIds.insert(std::make_pair(prop->getId(), prop));
			//}
			float value = 0.0;

			*iss >> value;
			//std::vector<std::shared_ptr<PropertyFieldDef>>::iterator it = find(propertyIds.begin(), propertyIds.end(), i);
			vertex->addProperty(currentIndex,value);
			scalarFieldDef->expandBounds(value);
			currentIndex++;
			//propertyIds[i]->expandBounds(value);
		}

};

#endif // PROPERTYFIELDLOADINGPLY_H
