#include "selectbyid.h"
#include "selectbyidconfig.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Element/Element.h"
#include "Common/Constants.h"
SelectById::SelectById():
	SelectionStrategy("Select By Id")
{
	config = (SelectByIdConfig*)0;
}
SelectById::~SelectById(){
	if(config)
		delete config;
}
bool SelectById::setup(){
	config->readConfig();
	applyOnSelection = config->selectionType == SelectByIdConfig::INTERSECT_SELECTION;
	return true;
}
bool SelectById::selectElement( vis::Polyhedron * e, Selection* sel){
	return selectElement((vis::Element*)e,sel);
}
bool SelectById::selectElement( vis::Polygon * e, Selection* sel){
	return selectElement((vis::Element*)e,sel);
}
bool SelectById::selectElement( vis::Vertex * e, Selection* sel){
	return selectElement((vis::Element*)e,sel);
}
bool SelectById::selectElement(vis::Element* el, Selection* sel){
	if(el->getId()>=config->minId && el->getId() <= config->maxId){
		sel->addSelectedElement(el);
		return true;
	}
	return false;
}

bool SelectById::isFullFilled( vis::Polyhedron *){
	return config->elementsType == vis::CONSTANTS::POLYHEDRON;
}
bool SelectById::isFullFilled( vis::Polygon *){
	return config->elementsType == vis::CONSTANTS::POLYGON;
}
bool SelectById::isFullFilled( vis::Vertex *){
	return config->elementsType == vis::CONSTANTS::VERTEX;
}


bool SelectById::hasConfigWidget(){
	return true;
}

void SelectById::QApplicationInitiated(){
	config = new SelectByIdConfig();
}

QWidget* SelectById::getConfigWidget(){
	return config;
}

bool SelectById::applyButtonEnabled(){
	return true;
}

bool SelectById::setupPreApplying(Selection* sel, RModel *){
	if(config->selectionType == SelectByIdConfig::NEW_SELECTION){
		sel->clearSelectedElements();
		sel->setSelectionType(config->elementsType);
		return true;
	}else{
		if(sel->getSelectionType()==Selection::NOT_INITIALIZED)
			return false;
		config->elementsType = sel->getSelectionType();
		return true;
	}
}

#include "Factories/selectionstrategyregistry.h"
REGISTER_SELECTION_STRATEGY(SelectById);
