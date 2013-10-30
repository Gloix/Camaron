#include "polyhedronutils.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
PolyhedronUtils::PolyhedronUtils(){}
void addUniqueElement(vis::Vertex* vertex,std::vector<vis::Vertex*>& vertices){
	for(std::vector<vis::Vertex*>::size_type j = 0; j < vertices.size();j++)
		if(vertices[j] == vertex)
			return;//already in vector
	vertices.push_back(vertex);
}

void PolyhedronUtils::getPolyhedronVertices(vis::Polyhedron* p, std::vector<vis::Vertex*>& ver){
	std::vector<vis::Polygon*>& polygons = p->getPolyhedronPolygons();
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<vis::Vertex*>& vertices = polygons[i]->getVertices();
		for(std::vector<vis::Vertex*>::size_type j = 0; j < vertices.size();j++)
			addUniqueElement(vertices[j],ver);
	}
}

void PolyhedronUtils::setPolyhedronRModelPositions(vis::Polyhedron* p){
	std::vector<int>& rmodelPos = p->getRmodelPositions();
	std::vector<vis::Polygon*>& polygons = p->getPolyhedronPolygons();
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		std::vector<int>& polygonRmodelPos = polygons[i]->getRmodelPositions();
		for(std::vector<int>::size_type j = 0;j<polygonRmodelPos.size();j++)
			rmodelPos.push_back(polygonRmodelPos[j]);
	}
}
float PolyhedronUtils::getPolyhedronSolidAngleFromVertex(vis::Polyhedron* p,
														 vis::Vertex* v){
	std::vector<vis::Polygon*> projectedFaces;
	std::vector<vis::Polygon*>& polygons = p->getPolyhedronPolygons();
	glm::vec3 geocenterToVertexV = v->getCoords()-p->getGeometricCenter();
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		if(glm::dot(p->getOutwardNormal(polygons[i]),
					geocenterToVertexV)<0)
			projectedFaces.push_back(polygons[i]);
	}
	float totalProjectedArea = 0.0f;
	for(std::vector<vis::Polygon*>::size_type i = 0;i<projectedFaces.size();i++){
		std::vector<vis::Vertex*>& polygonVertices = projectedFaces[i]->getVertices();
		if(polygonVertices.size()<=2)
			continue;
		std::vector<glm::vec3> translatedCoords;
		for(std::vector<vis::Vertex*>::size_type j = 0;
			j<polygonVertices.size();j++)
			translatedCoords.push_back(glm::normalize(polygonVertices[j]->getCoords()-
													  v->getCoords()));
		//For each vertex, its coordinates are projeted to a sphere centered in v
		glm::vec3& v0(translatedCoords[0]);
		//divide polygon into triangles
		for(std::vector<vis::Vertex*>::size_type j = 1;
			j<polygonVertices.size()-1;j++){
			glm::vec3& v1(translatedCoords[j]);
			glm::vec3& v2(translatedCoords[j+1]);
			//get angle between vertices of the spherical triangle
			float angleA = std::acos(glm::dot(v0,v1)/( v0.length()*v1.length()));
			float angleB = std::acos(glm::dot(v0,v2)/( v0.length()*v2.length()));
			float angleC = std::acos(glm::dot(v1,v2)/( v1.length()*v2.length()));
			float angleAlfa = std::acos((std::cos(angleA)-(std::cos(angleB)*std::cos(angleC)))/
										(std::sin(angleB)*std::sin(angleC)));
			float ratio = std::sin(angleAlfa)/std::sin(angleA);
			float angleBeta = std::asin(ratio*std::sin(angleB));
			float angleGamma = std::asin(ratio*std::sin(angleC));
			float Et = angleAlfa + angleBeta + angleGamma - PI;//spherical excess in radians

			totalProjectedArea+=(Et/(4*PI));
		}
	}
	return totalProjectedArea;
}

glm::vec3 PolyhedronUtils::getCoordsSphericalCoordinates(glm::vec3 cartesian){
	glm::vec3 sphericalCoords;
	sphericalCoords.x = cartesian.length();
	if(cartesian.z==0)
		sphericalCoords.y = PI/2.0f;
	else if(cartesian.z<0)
		sphericalCoords.y = PI + std::atan(std::sqrt((cartesian.x*cartesian.x)+
													 (cartesian.y*cartesian.y))/cartesian.z);
	else
		sphericalCoords.y = std::atan(std::sqrt((cartesian.x*cartesian.x)+
												(cartesian.y*cartesian.y))/cartesian.z);

	if(cartesian.x>0&&cartesian.y>0)
		sphericalCoords.z = std::atan(cartesian.y/cartesian.x);
	else if(cartesian.x>0&&cartesian.y<0)
		sphericalCoords.z = 2*PI+std::atan(cartesian.y/cartesian.x);
	else if(cartesian.x==0)
		sphericalCoords.z = PI/2.0f*((cartesian.y > 0) - (cartesian.y < 0));
	else if(cartesian.x<0)
		sphericalCoords.z = PI+std::atan(cartesian.y/cartesian.x);
	return sphericalCoords;
}
