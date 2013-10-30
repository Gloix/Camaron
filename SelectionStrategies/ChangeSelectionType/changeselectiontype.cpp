#include "changeselectiontype.h"
#include "changeselectiontypeconfig.h"
#include "SelectionStrategies/Selection.h"
#include "Common/Constants.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Rendering/RModel/rmodel.h"
ChangeSelectionType::ChangeSelectionType():
	SelectionStrategy("Change Type")
{
	config = (ChangeSelectionTypeConfig*)0;
	weight = SELECTION_STRATEGY_BASE_WEIGHT+10.0f;
	applyOnSelection = true;
}
ChangeSelectionType::~ChangeSelectionType()
{
	if(config)
		delete config;
}
void ChangeSelectionType::selectElement( Selection* sel ) {
	std::unordered_map<int,vis::Element*> newSelection;
	if(config->hint){//all mush be selected
		switch(sel->getSelectionType()){
			case vis::CONSTANTS::POLYGON:{
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It ){
					vis::Polygon* selected = (vis::Polygon*)( *It ).second;
					vis::Polyhedron** neighbors = selected->getNeighborPolyhedron();
					for(int i = 0;i<2;++i){
						if(!neighbors[i])
							i = 3;
						else{
							std::vector<vis::Polygon*>& polygons = neighbors[i]->getPolyhedronPolygons();
							bool addPolyhedron = true;
							for(std::vector<vis::Polygon*>::size_type j = 0;j<polygons.size();++j){
								if(!polygons[j]->isSelected()){//one polyhedron is not selected
									j = polygons.size();
									addPolyhedron = false;
								}
							}
							if(addPolyhedron)
								newSelection[neighbors[i]->getId()] = neighbors[i];
						}
					}
				}
				break;
			}
			case vis::CONSTANTS::VERTEX:{
				//Vertex* selected = (Vertex*)( *It ).second;
				break;
			}
			case vis::CONSTANTS::POLYHEDRON:{
				typedef std::vector<vis::Polygon*>::size_type size_type;
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It )
				{
					vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
					std::vector<vis::Polygon*>& polyhedronPolygons = selected->getPolyhedronPolygons();
					for(size_type k = 0;k<polyhedronPolygons.size();k++){
						vis::Polygon* currentPolygon = polyhedronPolygons[k];
						vis::Polyhedron* otherPolyhedron = currentPolygon->getNeighborPolyhedron(selected);
						bool addPolygon = true;
						if(otherPolyhedron)
							addPolygon = otherPolyhedron->isSelected();
						if(addPolygon)
							newSelection[currentPolygon->getId()] = currentPolygon;
					}
				}
				break;
			}
		}
	}
	else{
		switch(sel->getSelectionType()){
			case vis::CONSTANTS::POLYGON:{
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It ){
					vis::Polygon* selected = (vis::Polygon*)( *It ).second;
					vis::Polyhedron** neighbors = selected->getNeighborPolyhedron();
					if(neighbors[0]){
						newSelection[neighbors[0]->getId()] = neighbors[0];
						if(neighbors[1])
							newSelection[neighbors[1]->getId()] = neighbors[1];
					}
				}
				break;
			}
			case vis::CONSTANTS::VERTEX:{
				//Vertex* selected = (Vertex*)( *It ).second;
				break;
			}
			case vis::CONSTANTS::POLYHEDRON:{
				typedef std::vector<vis::Polygon*>::size_type size_type;
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It )
				{
					vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
					std::vector<vis::Polygon*>& polyhedronPolygons = selected->getPolyhedronPolygons();
					for(size_type k = 0;k<polyhedronPolygons.size();k++)
						newSelection[polyhedronPolygons[k]->getId()] = polyhedronPolygons[k];
				}
				break;
			}
		}
	}
	sel->clearSelectedElements();
	sel->setSelectionType(config->elementsType);
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelection.begin();
		  It != newSelection.end(); ++It ){
		sel->addSelectedElement(( *It ).second);
	}
}
bool ChangeSelectionType::isFullFilled( Selection* sel ) {
	return sel->getSelectionType()!=config->elementsType &&
									sel->getSelectionType() != Selection::NOT_INITIALIZED;
}
bool ChangeSelectionType::setup(){
	config->readConfig();
	return true;
}
bool ChangeSelectionType::setupPreApplying(Selection* sel, RModel * rmodel){
	if(sel->getSelectionType()==config->elementsType)
		return false;//same type, no change
	if(config->elementsType == vis::CONSTANTS::POLYGON){
		if(rmodel->getModelType() == vis::CONSTANTS::VERTEX_CLOUD)
			return false;//model has no polygons
	}
	if(config->elementsType == vis::CONSTANTS::POLYHEDRON){
		if(rmodel->getModelType() != vis::CONSTANTS::POLYHEDRON_MESH)
			return false;//model has no polyhedrons
	}
	return true;
}
bool ChangeSelectionType::hasConfigWidget(){
	return true;
}

QWidget* ChangeSelectionType::getConfigWidget(){
	return this->config;
}
void ChangeSelectionType::QApplicationInitiated(){
	config = new ChangeSelectionTypeConfig();
}

#include "Factories/selectionstrategyregistry.h"
REGISTER_SELECTION_STRATEGY(ChangeSelectionType);
