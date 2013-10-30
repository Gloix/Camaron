#include "maximumsolidangle.h"
#include <limits>
#include "Utils/polyhedronutils.h"
#include "Model/Element/Polyhedron.h"
#include <QIcon>
MaximumSolidAngle::MaximumSolidAngle():
	EvaluationStrategy("Maximum Solid Angle", "Maximum Solid Angle")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.670f;
}
MaximumSolidAngle::~MaximumSolidAngle()
{
}
float MaximumSolidAngle::value( vis::Polyhedron* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float maximumAngle = -std::numeric_limits<float>::max();
	std::vector<vis::Vertex*> polyhedronVertices;
	PolyhedronUtils::getPolyhedronVertices(m,polyhedronVertices);
	for( std::vector<vis::Vertex*>::size_type i = 0;
		 i != polyhedronVertices.size(); i++ ) {
		float currentAngle = PolyhedronUtils::getPolyhedronSolidAngleFromVertex(m,polyhedronVertices[i]);
		maximumAngle = std::max(maximumAngle,currentAngle);
	}
	m->addProperty(this->id, maximumAngle);
	this->addNewValue(maximumAngle);
	return maximumAngle;
}
bool MaximumSolidAngle::isFullFilled( vis::Polyhedron* ){
	return true;
}
float MaximumSolidAngle::getNullValue(){
	return -std::numeric_limits<float>::max();
}
void MaximumSolidAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MaximumSolidAngle);
