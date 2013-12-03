#include "EvaluationStrategies/PolyhedronArea.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include <QIcon>
PolyhedronArea::PolyhedronArea():
	EvaluationStrategy("Surface","Surface")
{
    //weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.5f;
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.1f;
}
PolyhedronArea::~PolyhedronArea()
{
}
float PolyhedronArea::value( vis::Polyhedron* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float totalArea = 0.0;

	std::vector<vis::Polygon*> &vecPol = m->getPolyhedronPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i != vecPol.size(); i++ ) {
		float areaPolygon = vecPol[i]->getArea();
		totalArea += areaPolygon;
	}
	m->addProperty(this->id, totalArea);
	this->addNewValue(totalArea);
	return totalArea;
}
bool PolyhedronArea::isFullFilled( vis::Polyhedron* ){
	return true;
}
float PolyhedronArea::getNullValue(){
	return -1.0f;
}
void PolyhedronArea::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolyhedronArea);
