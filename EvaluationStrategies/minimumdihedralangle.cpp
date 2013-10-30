#include "minimumdihedralangle.h"
#include "Model/Element/Polygon.h"
#include "Utils/PolygonUtils.h"
#include <limits>
#include <QIcon>
MinimumDihedralAngle::MinimumDihedralAngle():
	EvaluationStrategy("Minimum Dihedral Angle", "Minimum Dihedral Angle")
{
    //weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.565f;
    weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.5f;
}
MinimumDihedralAngle::~MinimumDihedralAngle()
{
}
float MinimumDihedralAngle::value( vis::Polygon* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float minimumAngle = std::numeric_limits<float>::max();
	std::vector<vis::Polygon*> &vecPol = m->getNeighborPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i != vecPol.size(); i++ ) {
		float currentAngle = PolygonUtils::getDihedralAngle(m,vecPol[i]);
		minimumAngle = std::min(minimumAngle,currentAngle);
	}
	m->addProperty(this->id, minimumAngle);
	this->addNewValue(minimumAngle);
	return minimumAngle;
}
bool MinimumDihedralAngle::isFullFilled( vis::Polygon* ){
	return true;
}
float MinimumDihedralAngle::getNullValue(){
	return std::numeric_limits<float>::max();
}
void MinimumDihedralAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MinimumDihedralAngle);
