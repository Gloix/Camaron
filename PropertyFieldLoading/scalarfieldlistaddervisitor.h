#ifndef SCALARFIELDLISTADDERVISITOR_H
#define SCALARFIELDLISTADDERVISITOR_H
#include "PropertyFieldLoading/propertyfielddefvisitor.h"

#include <vector>
#include <memory>

class ScalarFieldListAdderVisitor : public PropertyFieldDefVisitor
{
	public:
		ScalarFieldListAdderVisitor(std::vector<std::shared_ptr<ScalarFieldDef>>& vector);
		virtual ~ScalarFieldListAdderVisitor();
		void visit(std::shared_ptr<ScalarFieldDef>);

	private:
		std::vector<std::shared_ptr<ScalarFieldDef>>& vector;
};

#endif // SCALARFIELDLISTADDERVISITOR_H
