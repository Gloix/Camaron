#include "SelectionStrategies/SelectionStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Element/Element.h"
#include "Model/PolygonMesh.h"
#include "Model/VertexCloud.h"
#include "Model/PolyhedronMesh.h"
#include <Utils/crosstimer.h>
#include <iostream>


SelectionStrategy::SelectionStrategy(std::string newname):
	name(newname)
{
	applyOnSelection = false;
	weight = SELECTION_STRATEGY_BASE_WEIGHT;
}

SelectionStrategy::~SelectionStrategy()
{
	//dtor
}
bool SelectionStrategy::isFullFilled( vis::Vertex* ) { return false;}
bool SelectionStrategy::isFullFilled( vis::Polygon* ) { return false;}
bool SelectionStrategy::isFullFilled( vis::Polyhedron* ) { return false;}
bool SelectionStrategy::isFullFilled( vis::Edge* ) { return false;}
bool SelectionStrategy::isFullFilled( Selection* ) { return true;}
bool SelectionStrategy::selectElement( vis::Polyhedron *, Selection* ) {return false;}
bool SelectionStrategy::selectElement( vis::Polygon *, Selection* ) {return false;}
bool SelectionStrategy::selectElement( vis::Vertex *, Selection* ) {return false;}
bool SelectionStrategy::selectElement( vis::Edge *, Selection* ) {return false;}
void SelectionStrategy::selectElement( Selection* sel) {
	std::unordered_map<int, vis::Element*>& hash = sel->getSelectedElements();
	std::vector<vis::Element*> erasedKeys;
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = hash.begin(); It != hash.end(); ++It )
		if(!( *( *It ).second ).applySelectionStrategyDD(this,sel))
			erasedKeys.push_back((*It).second);
	for ( vis::Element* element : erasedKeys )
		sel->removeSelectedElement(element);
}
bool SelectionStrategy::setup(){return true;}
bool SelectionStrategy::setupPreApplying(Selection*, RModel*){return true;}

const char* SelectionStrategy::getName( ) const{
	return this->name.c_str();
}
bool  SelectionStrategy::hasConfigWidget(){
	return false;
}
QWidget*  SelectionStrategy::getConfigWidget(){
	return (QWidget*)0;
}
bool SelectionStrategy::isAppliedOnSelection(){
	return applyOnSelection;
}
bool SelectionStrategy::applyButtonEnabled(){
	return true;
}
bool SelectionStrategy::selectElementsFrom( PolyhedronMesh* p,RModel* rmodel,
											Selection* selection,
											bool setupPreApplied ){
	CrossTimer cr;
	if(!setupPreApplied){
		if(!setupPreApplying(selection,rmodel))
			return false;
		std::cout << "Setup Pre-applying time: " << cr.getTranscurredSeconds()<<std::endl;
	}
	if( isFullFilled((vis::Polyhedron*)0)){
		std::vector<vis::Polyhedron*>& polyhedrons = p->getPolyhedrons();
		for( vis::Polyhedron* polyhedron : polyhedrons )
			selectElement(polyhedron,selection);
		std::cout << "Strategy "<< name <<"Applying time: " << cr.getTranscurredSeconds()<<std::endl;
		return true;
	}
	else
		return selectElementsFrom((PolygonMesh*)p,rmodel,selection,true);
}
bool SelectionStrategy::selectElementsFrom( PolygonMesh* p,RModel* rmodel,
											Selection* selection,
											bool setupPreApplied){
	CrossTimer cr;
	if(!setupPreApplied){
		if(!setupPreApplying(selection,rmodel))
			return false;
		std::cout << "Setup Pre-applying time: " << cr.getTranscurredSeconds()<<std::endl;
	}
	if( isFullFilled((vis::Polygon*)0)){
		std::vector<vis::Polygon*>& polygons = p->getPolygons();
		for( vis::Polygon* polygon : polygons )
			selectElement(polygon,selection);
		std::cout << "Strategy "<< name <<"Applying time: " << cr.getTranscurredSeconds()<<std::endl;
		return true;
	}
	else
		return selectElementsFrom((VertexCloud*)p,rmodel,selection,true);
}

bool SelectionStrategy::selectElementsFrom( VertexCloud* p,
											RModel* rmodel,
											Selection* selection,
											bool setupPreApplied){
	return false;
	//Not ready
	CrossTimer cr;
	if(!setupPreApplied){
		if(!setupPreApplying(selection,rmodel))
			return false;
		std::cout << "Setup Pre-applying time: " << cr.getTranscurredSeconds()<<std::endl;
	}
	if( isFullFilled((vis::Vertex*)0)){
		std::vector<vis::Vertex*>& vertices = p->getVertices();
		for( vis::Vertex* vertex : vertices )
			selectElement(vertex,selection);
		std::cout << "Strategy "<< name <<"Applying time: " << cr.getTranscurredSeconds()<<std::endl;
		return true;
	}
	return false;
}
void SelectionStrategy::QApplicationInitiated(){

}
float SelectionStrategy::getWeight(){
	return weight;
}

