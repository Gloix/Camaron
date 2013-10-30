#include "EvaluationStrategies/PolygonArea.h"
#include "Model/Element/Polygon.h"
#include <QIcon>
PolygonArea::PolygonArea():
	EvaluationStrategy("Area", "Area")
{
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.5f;
}

PolygonArea::~PolygonArea()
{
}
float PolygonArea::value( vis::Polygon* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float area = m->getArea();
	m->addProperty(this->id, area);
	this->addNewValue(area);
	return area;
}
bool PolygonArea::isFullFilled( vis::Polygon* ) { return true;}

float PolygonArea::getNullValue(){
	return -1.0f;
}
void PolygonArea::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolygonArea);
