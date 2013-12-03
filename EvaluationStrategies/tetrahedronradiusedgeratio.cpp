#include "tetrahedronradiusedgeratio.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/triangle.h"
#include "Utils/polyhedronutils.h"
#include "Model/Element/Vertex.h"
#include <QIcon>
TetrahedronRadiusEdgeRatio::TetrahedronRadiusEdgeRatio():
	EvaluationStrategy("Tetrahedron Radius Ratio", "Tetrahedron Radius Ratio")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 2.0f;
}
TetrahedronRadiusEdgeRatio::~TetrahedronRadiusEdgeRatio()
{
}
float TetrahedronRadiusEdgeRatio::value( vis::Polyhedron* m ) {
	if(m->getPolygonsCount()!=4)
		return this->getNullValue();//not tetrahedron
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);

	std::vector<vis::Vertex*> vertices;
	PolyhedronUtils::getPolyhedronVertices(m,vertices);

	glm::vec3 &a = vertices[0]->getCoords();
	glm::vec3 &b = vertices[1]->getCoords();
	glm::vec3 &c = vertices[2]->getCoords();
	glm::vec3 &d = vertices[3]->getCoords();

	float div =	2* glm::determinant(glm::mat3(
					b-a,
					c-a,
					d-a));

	float top = glm::length(glm::pow(glm::length(d-a),2.0f)*glm::cross(b-a,c-a) +
							glm::pow(glm::length(c-a),2.0f)*glm::cross(d-a,b-a) +
							glm::pow(glm::length(b-a),2.0f)*glm::cross(c-a,d-a));
	float radius = glm::abs(top/div);
	float minLength = std::numeric_limits<float>::max();
	std::vector<vis::Polygon*> &triangles = m->getPolyhedronPolygons();
	for(std::vector<vis::Polygon*>::size_type i = 0;i<triangles.size();i++){
		vis::Triangle* current = (vis::Triangle*)triangles[i];
		minLength = std::min(minLength,current->getLMin());
	}
	float ratio = radius/minLength;
	m->addProperty(this->id, ratio);
	this->addNewValue(ratio);
	return ratio;
}
bool TetrahedronRadiusEdgeRatio::isFullFilled( vis::Polyhedron* ){
	return true;
}
float TetrahedronRadiusEdgeRatio::getNullValue(){
	return -std::numeric_limits<float>::max();
}
void TetrahedronRadiusEdgeRatio::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/tetrahedron.png");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(TetrahedronRadiusEdgeRatio);
