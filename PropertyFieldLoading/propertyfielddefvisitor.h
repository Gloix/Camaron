#ifndef PROPERTYFIELDDEFVISITOR
#define PROPERTYFIELDDEFVISITOR
#include <memory>

class ScalarFieldDef;
class TQSPropertyFieldDef;

class PropertyFieldDefVisitor
{
	public:
		virtual void visit(std::shared_ptr<ScalarFieldDef>) {}
		virtual void visit(std::shared_ptr<TQSPropertyFieldDef>) {}
};

#endif // PROPERTYFIELDDEFVISITOR

