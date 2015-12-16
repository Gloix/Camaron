#include "scalarfieldlistaddervisitor.h"

ScalarFieldListAdderVisitor::ScalarFieldListAdderVisitor(std::vector<std::shared_ptr<ScalarFieldDef>>& vector) :
	vector(vector)
{

}

ScalarFieldListAdderVisitor::~ScalarFieldListAdderVisitor() {
}

void ScalarFieldListAdderVisitor::visit(std::shared_ptr<ScalarFieldDef> prop){
	vector.push_back(prop);
}

