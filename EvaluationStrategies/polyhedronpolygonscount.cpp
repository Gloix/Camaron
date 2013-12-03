#include "polyhedronpolygonscount.h"
#include <QIcon>
#include "Model/Element/Polyhedron.h"
PolyhedronPolygonsCount::PolyhedronPolygonsCount():
	EvaluationStrategy("Polygons Count", "Polygons Count")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.9f;
}

PolyhedronPolygonsCount::~PolyhedronPolygonsCount()
{
}
float PolyhedronPolygonsCount::value( vis::Polyhedron* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float count = m->getPolygonsCount();
	m->addProperty(this->id, count);
	this->addNewValue(count);
	return count;
}
bool PolyhedronPolygonsCount::isFullFilled( vis::Polyhedron* ) { return true;}

float PolyhedronPolygonsCount::getNullValue(){
	return -1.0f;
}
void PolyhedronPolygonsCount::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolyhedronPolygonsCount);
