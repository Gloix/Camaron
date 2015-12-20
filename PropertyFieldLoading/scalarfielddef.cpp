#include "scalarfielddef.h"
#include <memory>

ScalarFieldDef::ScalarFieldDef(int id, std::string name, int elements) :
	PropertyFieldDef(id, name, elements),
	bounds(0)
{
}

ScalarFieldDef::ScalarFieldDef(int id, std::string name, int elements, float min, float max) :
	ScalarFieldDef(id, name, elements)
{
	bounds.resize(2);
	bounds[0] = min;
	bounds[1] = max;
}

int getElementSize() {
	return 1;
}


void ScalarFieldDef::expandBounds(float value)
{
	if(bounds.size() == 0)
	{
		bounds.resize(2);
		bounds[0] = value;
		bounds[1] = value;
		return;
	}
	if(bounds[0] > value)
	{
		bounds[0] = value;
	}
	else if (bounds[1] < value)
	{
		bounds[1] = value;
	}
}

float ScalarFieldDef::getMax() const
{
	if(bounds.size() == 0) {
		return 0;
	}
	return bounds[1];
}

float ScalarFieldDef::getMin() const
{
	if(bounds.size() == 0) {
		return 0;
	}
	return bounds[0];
}

int ScalarFieldDef::getElementSize() const {
	return 1;
}

DOUBLE_DISPATCH_PROPDEF_DEF(ScalarFieldDef)
