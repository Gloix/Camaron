#include "maximumdihedralangle.h"
#include "Model/Element/Polygon.h"
#include "Utils/PolygonUtils.h"
#include <limits>
#include <QIcon>
MaximumDihedralAngle::MaximumDihedralAngle():
	EvaluationStrategy("Maximum Dihedral Angle", "Maximum Dihedral Angle")
{
    //weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.57f;
	weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.57f;
}
MaximumDihedralAngle::~MaximumDihedralAngle()
{
}
float MaximumDihedralAngle::value( vis::Polygon* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float maximumAngle = -std::numeric_limits<float>::max();
	std::vector<vis::Polygon*> &vecPol = m->getNeighborPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i != vecPol.size(); i++ ) {
		float currentAngle = PolygonUtils::getDihedralAngle(m,vecPol[i]);
		maximumAngle = std::max(currentAngle,maximumAngle);
	}
	m->addProperty(this->id, maximumAngle);
	this->addNewValue(maximumAngle);
	return maximumAngle;
}
bool MaximumDihedralAngle::isFullFilled( vis::Polygon* ){
	return true;
}
float MaximumDihedralAngle::getNullValue(){
	return -std::numeric_limits<float>::max();
}
void MaximumDihedralAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MaximumDihedralAngle);
