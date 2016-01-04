#include "minimuminternalangle.h"
#include "Model/Element/Vertex.h"
#include "Utils/PolygonUtils.h"
#include <limits>
#include <QIcon>
#include <math.h>

MinimumInternalAngle::MinimumInternalAngle():
	EvaluationStrategy("Minimum Internal Angle", "Minimum Internal Angle")
{
	//weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.565f;
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.2f;
}
MinimumInternalAngle::~MinimumInternalAngle()
{
}
float MinimumInternalAngle::value( vis::Polygon* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float minimumAngle = std::numeric_limits<float>::max();
	std::vector<vis::Vertex*> &vec = m->getVertices();
	if(vec.size()<=2u){
		minimumAngle = 0.0;
	}else{
		for( decltype(vec.size()) i = 0; i != vec.size(); i++ ) {
			glm::vec3 u = vec[(i+2)%vec.size()]->getCoords()-vec[(i+1)%vec.size()]->getCoords();
			glm::vec3 v = vec[i]->getCoords()-vec[(i+1)%vec.size()]->getCoords();
			float dots = std::acos(glm::dot(u,v)/(glm::length(u)*glm::length(v)));
			minimumAngle = std::min(minimumAngle,dots);
		}
	}
	m->addProperty(this->id, minimumAngle);
	this->addNewValue(minimumAngle);
	return minimumAngle;
}
bool MinimumInternalAngle::isFullFilled( vis::Polygon* ){
	return true;
}
float MinimumInternalAngle::getNullValue(){
	return std::numeric_limits<float>::max();
}
void MinimumInternalAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MinimumInternalAngle);
