#include "SelectionStrategies/SelectionStrategyByProperty/selectionstrategybyproperty.h"
#include "SelectionStrategies/SelectionStrategyByProperty/selectionstrategybypropertyconfig.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Vertex.h"
#include "Common/Constants.h"
SelectionStrategyByProperty::SelectionStrategyByProperty():
	SelectionStrategy("Select by Property")
{
	config = (SelectionStrategyByPropertyConfig*)0;
	weight = SELECTION_STRATEGY_BASE_WEIGHT;
}

SelectionStrategyByProperty::~SelectionStrategyByProperty()
{
	if(config)
		delete config;
}
bool SelectionStrategyByProperty::selectElement( vis::Polyhedron * p, Selection* sel ) {
	if(!p->fullFillsEvaluationStrategy(evaluationStrategy))//P may be a tetrahedron
		return false;
	float value = p->evaluateUsingEvaluationStrategy(evaluationStrategy);
	if( value != evaluationStrategy->getNullValue() &&
			this->minVal <= value && this->maxVal >= value ){
		sel->addSelectedElement( p );
		return true;
	}
	return false;
}
bool SelectionStrategyByProperty::selectElement( vis::Polygon * p, Selection* sel ) {
	if(!p->fullFillsEvaluationStrategy(evaluationStrategy)) //P may be a triangle
		return false;
	float value = p->evaluateUsingEvaluationStrategy(evaluationStrategy);
	if(  value != evaluationStrategy->getNullValue() &&
		 this->minVal <= value && this->maxVal >= value ){
		sel->addSelectedElement( p );
		return true;
	}
	return false;
}
bool SelectionStrategyByProperty::selectElement( vis::Vertex * p, Selection * sel ) {
	if(!evaluationStrategy->isFullFilled(p))
		return false;
	float value = evaluationStrategy->value(p);
	if(  value != evaluationStrategy->getNullValue() &&
		 this->minVal <= value && this->maxVal >= value ){
		sel->addSelectedElement( p );
		return true;
	}
	return false;
}
//should i try with vertex
bool SelectionStrategyByProperty::isFullFilled( vis::Vertex * v) {
	return evaluationStrategy->isFullFilled(v);
}
bool SelectionStrategyByProperty::isFullFilled( vis::Polygon * p) {
	return evaluationStrategy->isFullFilled(p) || evaluationStrategy->isFullFilled((vis::Triangle*)0);
}
bool SelectionStrategyByProperty::isFullFilled( vis::Polyhedron * p) {
	return evaluationStrategy->isFullFilled(p);
}
bool SelectionStrategyByProperty::setup(){
	config->readConfig();
	this->minVal = config->minVal;
	this->maxVal = config->maxVal;
	this->evaluationStrategy = config->evaluationStrategy;
	this->applyOnSelection = !config->applyOverModel;
	return true;
}
bool SelectionStrategyByProperty::setupPreApplying(Selection* sel, RModel *){
	if(!this->applyOnSelection){
		if(config->newSelection){
			sel->clearSelectedElements();
			if(this->evaluationStrategy->isFullFilled((vis::Vertex*)0))
				sel->setSelectionType(vis::CONSTANTS::VERTEX);
			else if(this->evaluationStrategy->isFullFilled((vis::Polygon*)0)||
					this->evaluationStrategy->isFullFilled((vis::Triangle*)0))
				sel->setSelectionType(vis::CONSTANTS::POLYGON);
			else if(this->evaluationStrategy->isFullFilled((vis::Polyhedron*)0))
				sel->setSelectionType(vis::CONSTANTS::POLYHEDRON);
		}else{
			if(sel->getSelectedElements().empty()){
				if(this->evaluationStrategy->isFullFilled((vis::Vertex*)0))
					sel->setSelectionType(vis::CONSTANTS::VERTEX);
				else if(this->evaluationStrategy->isFullFilled((vis::Polygon*)0)||
						this->evaluationStrategy->isFullFilled((vis::Triangle*)0))
					sel->setSelectionType(vis::CONSTANTS::POLYGON);
				else if(this->evaluationStrategy->isFullFilled((vis::Polyhedron*)0))
					sel->setSelectionType(vis::CONSTANTS::POLYHEDRON);
			}else{
				switch(sel->getSelectionType()){
					case vis::CONSTANTS::VERTEX:{
						return this->evaluationStrategy->isFullFilled((vis::Vertex*)0);
					}
					case vis::CONSTANTS::POLYGON:{
						return this->evaluationStrategy->isFullFilled((vis::Polygon*)0) ||
								this->evaluationStrategy->isFullFilled((vis::Triangle*)0) ;
					}
					case vis::CONSTANTS::POLYHEDRON:{
						return this->evaluationStrategy->isFullFilled((vis::Polyhedron*)0);
					}
				}
			}
		}
	}
	return true;
}
bool SelectionStrategyByProperty::hasConfigWidget(){
	return true;
}

QWidget* SelectionStrategyByProperty::getConfigWidget(){
	return this->config;
}
void SelectionStrategyByProperty::QApplicationInitiated(){
	config = new SelectionStrategyByPropertyConfig();
}

#include "Factories/selectionstrategyregistry.h"
REGISTER_SELECTION_STRATEGY(SelectionStrategyByProperty);
