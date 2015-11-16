#include "trianglequalitymeasure1.h"
#include "Model/Element/triangle.h"
#include <QIcon>
TriangleQualityMeasure1::TriangleQualityMeasure1():
	EvaluationStrategy("Bhatia and Lawrence TM","Bhatia and Lawrence")
{
	weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.5f;
}

float TriangleQualityMeasure1::value( vis::Triangle* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float area = m->getArea();
	float prop = 4*1.73205080757f*area/(m->getLMin()*m->getLMin()+
										m->getLMid()*m->getLMid()+
										m->getLMax()*m->getLMax());
	m->addProperty(this->id, prop);
	this->addNewValue(prop);
	return prop;
}
bool TriangleQualityMeasure1::isFullFilled( vis::Triangle* ) { return true;}
float TriangleQualityMeasure1::getNullValue(){
	return -1.0f;
}
void TriangleQualityMeasure1::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/triangle-outline-16.ico");
}


#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(TriangleQualityMeasure1);
