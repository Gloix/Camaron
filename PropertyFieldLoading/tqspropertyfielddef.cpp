#include "tqspropertyfielddef.h"
#include "Exceptions/exceptionmessage.h"

TQSPropertyFieldDef::TQSPropertyFieldDef(int id, std::string name, int elements, int coordinatesSystem, int dimensions) :
	PropertyFieldDef(id, name, elements),
	coordinatesSystem(coordinatesSystem),
	dimensions(dimensions)
{
}

void TQSPropertyFieldDef::addPolygonCoordinates(int sides, std::vector<std::vector<float>> values)
{
	if(1==valuesPerPolygonType.count(dimensions))
	{
		throw ExceptionMessage("File has two coordinate definitions for the same type of polygon!");
	}
	valuesPerPolygonType.insert(std::pair<int,std::vector<std::vector<float>>>(dimensions, values));
}

std::vector<std::vector<float>>* TQSPropertyFieldDef::getCoordinatesForPolygon(int sides)
{
	if(0 == valuesPerPolygonType.count(sides)) {
		return NULL;
	}
	return &valuesPerPolygonType[sides];
}

DOUBLE_DISPATCH_PROPDEF_DEF(TQSPropertyFieldDef)
