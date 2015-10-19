#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Utils/sortsearchutils.h"
#include <limits>
#include <algorithm>
#include "Model/PolygonMesh.h"
#include "Model/VertexCloud.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include <QIcon>


EvaluationStrategy::EvaluationStrategy(std::string newname,std::string newdescription,
									   std::string newPropertyName,
									   unsigned char idn):
	description(newdescription),
	name(newname),
	propertyName(newPropertyName)
{
	initVariables(idn);
}

EvaluationStrategy::EvaluationStrategy(std::string newname,
				   std::string newPropertyName,
				   unsigned char idn):
	description("No description\navailable."),
	name(newname),
	propertyName(newPropertyName)
{
	initVariables(idn);
}

void EvaluationStrategy::initVariables(int idn){
	this->id = idn;
	qicon = 0;
	this->valuesNeedSorting = false;
	this->totalElementsCount = 0;
	this->minValue = std::numeric_limits<float>::max();
	this->maxValue = -std::numeric_limits<float>::max();
	saveStatics = true;
	weight = VERTEX_CRITERIA_WEIGHT_BASE;
}

EvaluationStrategy::~EvaluationStrategy()
{
	if(qicon)
		delete qicon;
}
float EvaluationStrategy::value( vis::Vertex* ) { return getNullValue();}
float EvaluationStrategy::value( vis::Polygon* ) { return getNullValue();}
float EvaluationStrategy::value( vis::Triangle* t) { return value((vis::Polygon*)t);}
float EvaluationStrategy::value( vis::Polyhedron* ) { return getNullValue();}
bool EvaluationStrategy::isFullFilled( vis::Vertex* ) { return false;}
bool EvaluationStrategy::isFullFilled( vis::Polygon* ) { return false;}
bool EvaluationStrategy::isFullFilled( vis::Triangle* t) { return isFullFilled((vis::Polygon*)t);}
bool EvaluationStrategy::isFullFilled( vis::Polyhedron* ) { return false;}
bool EvaluationStrategy::isFullFilled( vis::Edge* ) { return false;}
const char* EvaluationStrategy::getName( ){
	return this->name.c_str();
}
const char* EvaluationStrategy::getPropertyName(){
	return this->propertyName.c_str();
}
void EvaluationStrategy::setTotalElementsCount(int n){
	if(!saveStatics)
		return;
	this->totalElementsCount = n;
	this->values.clear();
	if(n!=0)
		this->values.reserve(n);
	else
		std::vector<float>().swap(values);
	this->minValue = std::numeric_limits<float>::max();
	this->maxValue = -std::numeric_limits<float>::max();
}
int EvaluationStrategy::getTotalElementsCount(){
	return this->totalElementsCount;
}
void EvaluationStrategy::addNewValue(float val){
	this->values.push_back(val);
	this->minValue = std::min(val,this->minValue);
	this->maxValue = std::max(val,this->maxValue);
	this->valuesNeedSorting = true;
}
int EvaluationStrategy::getCalculatedValuesCount(){
	return this->values.size();
}
int EvaluationStrategy::getValuesCountInRange(float from, float to){
	if(from>to)
		return 0;
	if(values.size()==0)
		return 0;
	if(this->valuesNeedSorting){
		this->valuesNeedSorting = false;
		std::sort(values.begin(),values.end());
	}
	int posFrom = SortSearchUtils::binarySearchPosition(values,from);
	int posTo = SortSearchUtils::binarySearchPosition(values,to);
	return posTo-posFrom;
}
float EvaluationStrategy::getMinValue(){
	return minValue;
}

float EvaluationStrategy::getMaxValue(){
	return maxValue;
}

void EvaluationStrategy::setId(unsigned char idn){
	id = idn;
}
void EvaluationStrategy::setup( PolyhedronMesh* p ){
	setup((PolygonMesh*)p);
}

void EvaluationStrategy::setup( PolygonMesh* p){
	if(isFullFilled((vis::Polygon*)0) ||
			isFullFilled((vis::Triangle*)0))
		setTotalElementsCount(p->getPolygonsCount());
	else
		setup((VertexCloud*)p);
}

void EvaluationStrategy::setup( VertexCloud* p ){
	if(isFullFilled((vis::Vertex*)0))
		setTotalElementsCount(p->getVerticesCount());
}

void EvaluationStrategy::evaluateElementsFrom( PolyhedronMesh* p, std::vector<float>& values){
	bool savePropertyToArray = values.size()!=0;
	if( !isFullFilled( ( vis::Polyhedron* )0 ) ){
		evaluateElementsFrom((PolygonMesh*)p,values);//parent implementation
		return;
	}
	std::vector<vis::Polyhedron*> &vec = p->getPolyhedrons();
	for( std::vector<vis::Polyhedron*>::size_type i = 0; i != vec.size(); i++ ) {
		float value = vec[i]->evaluateUsingEvaluationStrategy(this);
		if(savePropertyToArray){
			std::vector<int>& rmodelPositions = vec[i]->getRmodelPositions();
			for(std::vector<int>::size_type j = 0;j < rmodelPositions.size();j++){
				values[rmodelPositions[j]] = value;
			}
		}
	}
}

void EvaluationStrategy::evaluateElementsFrom( PolygonMesh* p, std::vector<float>& values ){
	bool savePropertyToArray = values.size()!=0;
	if( !isFullFilled( ( vis::Polygon* )0 ) && !isFullFilled( ( vis::Triangle* )0 )  ){
		evaluateElementsFrom((VertexCloud*)p,values);//parent implementation
		return;
	}
	std::vector<vis::Polygon*> &vec = p->getPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i != vec.size(); i++ ) {
		float value = vec[i]->evaluateUsingEvaluationStrategy(this);
		if(savePropertyToArray){
			std::vector<int>& rmodelPositions = vec[i]->getRmodelPositions();
			for(std::vector<int>::size_type j = 0;j < rmodelPositions.size();j++){
				values[rmodelPositions[j]] = value;
			}
		}
	}
}

void EvaluationStrategy::evaluateElementsFrom( VertexCloud* p, std::vector<float>& values){
	bool savePropertyToArray = values.size()!=0;
	if( !isFullFilled( ( vis::Vertex* )0 ) ){
		if(savePropertyToArray)
			for(std::vector<float>::size_type j = 0;j < values.size();j++)
				values[j] = getNullValue();
		return;
	}
	std::vector<vis::Vertex*> &vec = p->getVertices();
	for( std::vector<vis::Vertex*>::size_type i = 0; i != vec.size(); i++ ) {
		float value = vec[i]->evaluateUsingEvaluationStrategy(this);
		if(savePropertyToArray){
			std::vector<int>& rmodelPositions = vec[i]->getRmodelPositions();
			for(std::vector<int>::size_type j = 0;j < rmodelPositions.size();j++){
				values[rmodelPositions[j]] = value;
			}
		}
	}
}
bool EvaluationStrategy::hasStatics(){
	return saveStatics;
}
float EvaluationStrategy::getWeight(){
	return weight;
}
void EvaluationStrategy::setWeight(float f){
	weight = f;
}
std::string EvaluationStrategy::getDescription(){
	return description;
}
bool EvaluationStrategy::hasDescriptionQWidget(){
	return false;
}

QWidget* EvaluationStrategy::getEvaluationStrategyDescriptionQWidget(){
	return (QWidget*)0;
}
void EvaluationStrategy::QApplicationInitiatedEv(){}

bool EvaluationStrategy::hasQIcon(){
	return qicon;
}

QIcon* EvaluationStrategy::getEvaluationStrategyQIcon(){
	return qicon;
}
