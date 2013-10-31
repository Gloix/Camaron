#include "maximuminternalangle.h"
#include "Model/Element/Vertex.h"
#include "Utils/PolygonUtils.h"
#include <limits>
#include <QIcon>
#include <math.h>

MaximumInternalAngle::MaximumInternalAngle():
	EvaluationStrategy("Maximum Internal Angle", "Maximum Internal Angle")
{
	//weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.565f;
	weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.615f;
}
MaximumInternalAngle::~MaximumInternalAngle()
{
}
float MaximumInternalAngle::value( vis::Polygon* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float maximumAngle = -std::numeric_limits<float>::max();
	std::vector<vis::Vertex*> &vec = m->getVertices();
	if(vec.size()<=2u){
		maximumAngle = 360.0f;
	}else{
		for( std::vector<vis::Vertex*>::size_type i = 0; i != vec.size(); i++ ) {
			glm::vec3 u = vec[(i+2)%vec.size()]->getCoords()-vec[(i+1)%vec.size()]->getCoords();
			glm::vec3 v = vec[i]->getCoords()-vec[(i+1)%vec.size()]->getCoords();
			float dots = std::acos(glm::dot(u,v)/(glm::length(u)*glm::length(v)));
			maximumAngle = std::max(maximumAngle,dots);
		}
	}
	m->addProperty(this->id, maximumAngle);
	this->addNewValue(maximumAngle);
	return maximumAngle;
}
bool MaximumInternalAngle::isFullFilled( vis::Polygon* ){
	return true;
}
float MaximumInternalAngle::getNullValue(){
	return -std::numeric_limits<float>::max();
}
void MaximumInternalAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MaximumInternalAngle);
