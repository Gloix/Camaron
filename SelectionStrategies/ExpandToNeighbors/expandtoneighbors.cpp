#include "expandtoneighbors.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/PolygonUtils.h"
#include "Common/Constants.h"
#include "SelectionStrategies/ExpandToNeighbors/expandtoneighborsconfig.h"
#include <limits>
ExpandToNeighbors::ExpandToNeighbors():SelectionStrategy("Expand Selection"){
	applyOnSelection = true;
	weight = SELECTION_STRATEGY_BASE_WEIGHT;
	config = (ExpandToNeighborsConfig*)0;
}
ExpandToNeighbors::~ExpandToNeighbors(){
	if(config)
		delete config;
}


void ExpandToNeighbors::selectElement( Selection * sel ) {
	if(config->onlySurface){
		expandSelectionSurface(sel);
		return;
	}
	if(config->onlySurfaceWithAngle){
		expandSelectionSurfaceWithAngle(sel,config->dihedralAngle);
		return;
	}
	this->expandSelectionAll(sel);
}
void ExpandToNeighbors::expandSelectionSurface(Selection* sel){
	std::unordered_map<int, vis::Element*>& selectedElements = sel->getSelectedElements();
	std::unordered_map<int, vis::Element*> newSelectedElements;
	int selectionType = sel->getSelectionType();
	switch(selectionType){
		case vis::CONSTANTS::POLYGON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polygon* selected = (vis::Polygon*)( *It ).second;
				std::vector<vis::Polygon*>& neighborPolygons = selected->getNeighborPolygons();
				for(std::vector<vis::Polygon*>::size_type i = 0;i<neighborPolygons.size();i++)
					if(neighborPolygons[i]->isAtSurface()&&
							!neighborPolygons[i]->isSelected())
						newSelectedElements[neighborPolygons[i]->getId()] = neighborPolygons[i];
			}
			break;
		}
		case vis::CONSTANTS::VERTEX:{
			//Vertex* selected = (Vertex*)( *It ).second;
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			typedef std::vector<vis::Polygon*>::size_type size_type;
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
				std::vector<vis::Polygon*>& polyhedronPolygons = selected->getPolyhedronPolygons();
				for(size_type k = 0;k<polyhedronPolygons.size();k++){
					if(!polyhedronPolygons[k]->isAtSurface())
						continue;
					std::vector<vis::Polygon*>& neighborPolygons = polyhedronPolygons[k]->getNeighborPolygons();
					for(size_type i = 0;i<neighborPolygons.size();i++){
						if(!neighborPolygons[i]->isAtSurface())
							continue;
						vis::Polyhedron* polAsoc = neighborPolygons[i]->getNeighborPolyhedron((vis::Polyhedron*)0);
						if(!polAsoc->isSelected())
							newSelectedElements[polAsoc->getId()] = polAsoc;
					}

				}
			}
			break;
		}
	}
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelectedElements.begin(); It != newSelectedElements.end(); ++It )
	{
		sel->addSelectedElement(( *It ).second);
	}
}

void ExpandToNeighbors::expandSelectionSurfaceWithAngle(Selection* sel, float angle){
	std::unordered_map<int, vis::Element*>& selectedElements = sel->getSelectedElements();
	std::unordered_map<int, vis::Element*> newSelectedElements;
	int selectionType = sel->getSelectionType();
	switch(selectionType){
		case vis::CONSTANTS::POLYGON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polygon* selected = (vis::Polygon*)( *It ).second;
				std::vector<vis::Polygon*>& neighborPolygons = selected->getNeighborPolygons();
				for(std::vector<vis::Polygon*>::size_type i = 0;i<neighborPolygons.size();i++)
					if(neighborPolygons[i]->isAtSurface()&&
							!neighborPolygons[i]->isSelected() &&
							PolygonUtils::getDihedralAngle(selected,neighborPolygons[i]) <= angle)
						newSelectedElements[neighborPolygons[i]->getId()] = neighborPolygons[i];
			}
			break;
		}
		case vis::CONSTANTS::VERTEX:{
			//Vertex* selected = (Vertex*)( *It ).second;
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			typedef std::vector<vis::Polygon*>::size_type size_type;
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
				std::vector<vis::Polygon*>& polyhedronPolygons = selected->getPolyhedronPolygons();
				for(size_type k = 0;k<polyhedronPolygons.size();k++){
					if(!polyhedronPolygons[k]->isAtSurface())
						continue;
					std::vector<vis::Polygon*>& neighborPolygons = polyhedronPolygons[k]->getNeighborPolygons();
					for(size_type i = 0;i<neighborPolygons.size();i++){
						if(!neighborPolygons[i]->isAtSurface())
							continue;
						vis::Polyhedron* polAsoc = neighborPolygons[i]->getNeighborPolyhedron((vis::Polyhedron*)0);
						if(!polAsoc->isSelected() &&
								PolygonUtils::getDihedralAngle(polyhedronPolygons[k],neighborPolygons[i]) >= angle)
							newSelectedElements[polAsoc->getId()] = polAsoc;
					}

				}
			}
			break;
		}
	}
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelectedElements.begin(); It != newSelectedElements.end(); ++It )
	{
		sel->addSelectedElement(( *It ).second);
	}
}

void ExpandToNeighbors::expandSelectionAll(Selection* sel){
	std::unordered_map<int, vis::Element*>& selectedElements = sel->getSelectedElements();
	std::unordered_map<int, vis::Element*> newSelectedElements;
	int selectionType = sel->getSelectionType();
	switch(selectionType){
		case vis::CONSTANTS::POLYGON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polygon* selected = (vis::Polygon*)( *It ).second;
				std::vector<vis::Polygon*>& neighborPolygons = selected->getNeighborPolygons();
				for(std::vector<vis::Polygon*>::size_type i = 0;i<neighborPolygons.size();i++)
					if(!neighborPolygons[i]->isSelected())
						newSelectedElements[neighborPolygons[i]->getId()] = neighborPolygons[i];
			}
			break;
		}
		case vis::CONSTANTS::VERTEX:{
			//Vertex* selected = (Vertex*)( *It ).second;
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
				std::vector<vis::Polygon*>& polygons = selected->getPolyhedronPolygons();
				for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
					vis::Polyhedron* neighbor = polygons[i]->getNeighborPolyhedron(selected);
					if(neighbor && !neighbor->isSelected())
						newSelectedElements[neighbor->getId()] = neighbor;
				}
			}
			break;
		}
	}
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelectedElements.begin(); It != newSelectedElements.end(); ++It )
	{
		sel->addSelectedElement(( *It ).second);
	}
}

bool ExpandToNeighbors::isFullFilled( Selection * ) {
	return true;
}
bool ExpandToNeighbors::setup(){
	config->readConfig();
	return true;
}
bool ExpandToNeighbors::setupPreApplying(Selection*, RModel *){
	return true;
}
bool ExpandToNeighbors::hasConfigWidget(){
	return true;
}

QWidget* ExpandToNeighbors::getConfigWidget(){
	return this->config;
}
void ExpandToNeighbors::QApplicationInitiated(){
	config = new ExpandToNeighborsConfig();
}
#include "Factories/selectionstrategyregistry.h"
REGISTER_SELECTION_STRATEGY(ExpandToNeighbors);
