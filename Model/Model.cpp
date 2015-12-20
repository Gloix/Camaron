#include "Model/Model.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "Exceptions/propertydefnotfoundexception.h"
#include "Rendering/Renderers/baserendererconfig.h"

Model::Model(std::string f):
	filename(f)
{
	//ctor
}

Model::~Model()
{
	//dtor
}
std::vector<float>& Model::getBounds() {
	return bounds;
}
void Model::evaluateUsingEvaluationStrategy( EvaluationStrategy* strategy ) {
	std::vector<float> empty;
	this->evaluateUsingEvaluationStrategy(strategy,empty);
}
int Model::getModelType(){
	return modelType;
}
std::string Model::getFilename(){
	return filename;
}
bool Model::isLightWeight(){
	return modelType==vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH||
			modelType==vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH||
			modelType==vis::CONSTANTS::LIGHT_WEIGHT_VERTEX_CLOUD;
}

void Model::clean(){}

void Model::addPropertyFieldDef(std::shared_ptr<PropertyFieldDef> propertyFieldDef) {
	propertyFieldDefs.push_back(propertyFieldDef);
	propertyFieldPositions[propertyFieldDef.get()] = propertyFieldDefs.size()-1;
}

unsigned char Model::getPropertyFieldPosition(PropertyFieldDef* pfd) {
	if(!propertyFieldPositions.count(pfd)) {
		throw new PropertyDefNotFoundException;
	}
	return propertyFieldPositions[pfd];
}

std::vector<std::shared_ptr<PropertyFieldDef>>& Model::getPropertyFieldDefs() {
	return propertyFieldDefs;
}
