#include "polygonaspectratio.h"
#include "Model/Element/triangle.h"
#include "Model/Element/Vertex.h"
#include <QIcon>
PolygonAspectRatio::PolygonAspectRatio():
	EvaluationStrategy("Aspect Ratio","Aspect ratio is calculated dividing the shortest edge by the longest edge of the polygon.\n AR = EdgeMin/EdgeMax", "Aspect Ratio")
{
    //weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.5f;
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.6f;

}
PolygonAspectRatio::~PolygonAspectRatio()
{
}

float PolygonAspectRatio::value(vis::Polygon* m ){
	if(m->hasProperty(id))
		return m->getProperty(id);
	std::vector<vis::Vertex*>& vertices = m->getVertices();
	float min,max;
	int verticesSizeMinusOne = vertices.size()-1;
	min = glm::distance(vertices[verticesSizeMinusOne]->getCoords(),
						vertices[0]->getCoords());
	max = min;
	for(int i = 0;i<verticesSizeMinusOne;i++){
		float d = glm::distance(vertices[i]->getCoords(),vertices[i+1]->getCoords());
		min = std::min(min,d);
		max = std::max(max,d);
	}
	float ratio = min/max;
	this->addNewValue(ratio);
	m->addProperty(this->id, ratio);
	return ratio;
}

bool PolygonAspectRatio::isFullFilled(vis::Polygon* ){
	return true;
}

float PolygonAspectRatio::value(vis::Triangle* m ){
	if(m->hasProperty(id))
		return m->getProperty(id);
	float ratio = m->getLMin()/m->getLMax();
	this->addNewValue(ratio);
	m->addProperty(this->id, ratio);
	return ratio;
}

bool PolygonAspectRatio::isFullFilled(vis::Triangle*){
	return true;
}

float PolygonAspectRatio::getNullValue(){
	return -1.0f;
}
void PolygonAspectRatio::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolygonAspectRatio);
