#include "minimumsolidangle.h"
#include <limits>
#include "Utils/polyhedronutils.h"
#include "Model/Element/Polyhedron.h"
#include <QIcon>
MinimumSolidAngle::MinimumSolidAngle():
	EvaluationStrategy("Minimum Solid Angle", "Minimum Solid Angle")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.3f;
}
MinimumSolidAngle::~MinimumSolidAngle()
{
}
float MinimumSolidAngle::value( vis::Polyhedron* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float minimumAngle = std::numeric_limits<float>::max();
	std::vector<vis::Vertex*> polyhedronVertices;
	PolyhedronUtils::getPolyhedronVertices(m,polyhedronVertices);
	for( vis::Vertex* vertex : polyhedronVertices ) {
		float currentAngle = PolyhedronUtils::getPolyhedronSolidAngleFromVertex(m,vertex);
		minimumAngle = std::min(minimumAngle,currentAngle);
	}
	m->addProperty(this->id, minimumAngle);
	this->addNewValue(minimumAngle);
	return minimumAngle;
}
bool MinimumSolidAngle::isFullFilled( vis::Polyhedron* ){
	return true;
}
float MinimumSolidAngle::getNullValue(){
	return std::numeric_limits<float>::max();
}
void MinimumSolidAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MinimumSolidAngle);
