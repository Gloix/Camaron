#include "propertyfielddef.h"

PropertyFieldDef::PropertyFieldDef(int id, std::string name, unsigned int elements) :
	id(id), name(name), elements(elements)
{
}

int PropertyFieldDef::getId() const {
	return id;
}

std::string PropertyFieldDef::getName() const {
	return name;
}

int PropertyFieldDef::getElementsCount() const {
	return elements;
}
