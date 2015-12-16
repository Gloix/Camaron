#ifndef PROPERTYFIELDDEF_H
#define PROPERTYFIELDDEF_H

#include "PropertyFieldLoading/propertyfielddefvisitor.h"
#include <string>
#include <memory>

#define PROPERTY_FIELD_VECTOR = 0;
#define PROPERTY_FIELD_GAUSS = 1;

#define DOUBLE_DISPATCH_PROPDEF_DEC0 \
	virtual void accept( PropertyFieldDefVisitor& ) = 0;

#define DOUBLE_DISPATCH_PROPDEF_DEC \
	virtual void accept( PropertyFieldDefVisitor& );

#define DOUBLE_DISPATCH_PROPDEF_DEF(x) \
	void x::accept( PropertyFieldDefVisitor& visitor) {\
		visitor.visit(shared_from_this());\
	}

class PropertyFieldDef
{
	public:
		PropertyFieldDef(int id, std::string name, unsigned int elements);
		int getId() const;
		std::string getName() const;
		int getElementsCount() const;
		virtual int getElementSize() const = 0; // number of floats
		DOUBLE_DISPATCH_PROPDEF_DEC0
	private:
		int id;
		std::string name;
		unsigned int elements;
};

#endif // PROPERTYFIELDDEF_H
