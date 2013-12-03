#include "polygonverticescount.h"
#include "Model/Element/Polygon.h"
#include <QIcon>
PolygonVerticesCount::PolygonVerticesCount():
	EvaluationStrategy("Vertices Count", "Vertices Count")
{
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.7f;
}

PolygonVerticesCount::~PolygonVerticesCount()
{
}
float PolygonVerticesCount::value( vis::Polygon* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float count = m->getVerticesCount();
	m->addProperty(this->id, count);
	this->addNewValue(count);
	return count;
}
bool PolygonVerticesCount::isFullFilled( vis::Polygon* ) { return true;}

float PolygonVerticesCount::getNullValue(){
	return -1.0f;
}
void PolygonVerticesCount::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolygonVerticesCount);
