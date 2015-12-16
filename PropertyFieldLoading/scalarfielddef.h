#ifndef SCALARFIELDDEF_H
#define SCALARFIELDDEF_H

#include<vector>

#include "PropertyFieldLoading/propertyfielddef.h"

class ScalarFieldDef : public PropertyFieldDef, public std::enable_shared_from_this<ScalarFieldDef>
{
	public:
		ScalarFieldDef(int id, std::string name, int elements);
		ScalarFieldDef(int id, std::string name, int elements, float min, float max);
		void expandBounds(float value);
		float getMax() const;
		float getMin() const;
		virtual int getElementSize() const; // number of floats
		DOUBLE_DISPATCH_PROPDEF_DEC
	private:
		std::vector<float> bounds;
};

#endif // SCALARFIELDDEF_H
