#include "Model/Element/Element.h"
vis::Element::Element( int newId )
{
	this->id = newId;
	this->selected = false;
}

vis::Element::~Element() {}

int vis::Element::getId() {
	return this->id;
}
bool vis::Element::isSelected() {
	return this->selected;
}
void vis::Element::setSelected( bool s ) {
	this->selected = s;
}
std::vector<int>& vis::Element::getRmodelPositions(){
	return rmodelPositions;
}
LowMemoryHash<unsigned char, float>& vis::Element::getProperties(){
	return this->properties;
}
bool vis::Element::hasProperty(int key){
	return this->properties.hasItem(key);
}
float vis::Element::getProperty(int key){
	LowMemoryHashItem<unsigned char,float>* item = properties.getItem(key);
	if(item&&item->isValid())
		return item->value;
	return 0.0f;
}
void vis::Element::addProperty(int key, float property){
	properties.insert(key,property);
}

void vis::Element::addRmodelPositions(int pos){
	rmodelPositions.push_back(pos);
}
