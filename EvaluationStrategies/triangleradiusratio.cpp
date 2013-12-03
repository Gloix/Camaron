#include "triangleradiusratio.h"
#include "Model/Element/triangle.h"
#include <QIcon>
TriangleRadiusRatio::TriangleRadiusRatio():
	EvaluationStrategy("Radius Ratio","Radius Ratio")
{
    //weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.5f;
	weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.5f;
	descriptionWidget = 0;
}
TriangleRadiusRatio::~TriangleRadiusRatio(){
	if(descriptionWidget)
		delete descriptionWidget;
}
float TriangleRadiusRatio::value( vis::Triangle* t) {
	if(t->hasProperty(this->id))
		return t->getProperty(this->id);
	float radiusRatio = 16*t->getArea()*t->getArea()/
						(t->getLMax()*t->getLMid()*t->getLMin()*
						 (t->getLMax()+t->getLMid()+t->getLMin()));
	t->addProperty(this->id, radiusRatio);
	this->addNewValue(radiusRatio);
	return radiusRatio;
}
bool TriangleRadiusRatio::isFullFilled( vis::Triangle* t) {
	return true;
}
float TriangleRadiusRatio::getNullValue(){
	return -1.0f;
}
bool TriangleRadiusRatio::hasDescriptionQWidget(){
	return true;
}

QWidget* TriangleRadiusRatio::getEvaluationStrategyDescriptionQWidget(){
	return descriptionWidget;
}
void TriangleRadiusRatio::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/triangle-outline-16.ico");
	descriptionWidget = new QWidget((QWidget*)0,
									Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
}


#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(TriangleRadiusRatio);

