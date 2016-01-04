#include "Utils/PolygonUtils.h"
#include "Utils/Printer.h"
#include "Model/LWElements/lwvertex.h"
#include "Model/LWElements/lwpolyhedron.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Common/Constants.h"
#include "Rendering/RModel/RVertexFlagAttribute.h"
#include <iostream>

PolygonUtils::PolygonUtils() {}
PolygonUtils::~PolygonUtils() {}

void PolygonUtils::getTriangleIndices(vis::Polygon* p,std::vector<GLuint>& indices){
	std::vector<vis::Vertex*> &vecVertices = p->getVertices();
	int firstIndex = vecVertices[0]->getPos();
	if(p->getVerticesCount() >= 3){
		for( std::vector<vis::Vertex*>::size_type i = 1; i < vecVertices.size() - 1; i++ ) {
			indices.push_back(firstIndex);
			indices.push_back(vecVertices[i]->getPos());
			indices.push_back(vecVertices[i+1]->getPos());
		}
	}
}
void PolygonUtils::configRVertexFlagAttribute(vis::Element* element, RVertexFlagAttribute& attribute){
	if(element->getRmodelPositions().size()!=1u)
		attribute.enableFlag(RVertexFlagAttribute::VIRTUAL_VERTEX);
	else
		attribute.disableFlag(RVertexFlagAttribute::VIRTUAL_VERTEX);
	if(element->isAtSurface())
		attribute.enableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
	else
		attribute.disableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
}
void PolygonUtils::configRVertexFlagAttribute(vis::LWVertex* element, RVertexFlagAttribute& attribute){
	if(element->isUsed())
		attribute.enableFlag(RVertexFlagAttribute::VIRTUAL_VERTEX);
	else{
		attribute.disableFlag(RVertexFlagAttribute::VIRTUAL_VERTEX);
		element->used();
	}
	if(element->isAtSurface())
		attribute.enableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
	else
		attribute.disableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
}
void addIds( vis::LWPolygon* p, std::vector<int>& ids){
	ids.push_back(p->getId());
	vis::LWPolyhedron** neighbors = p->getNeighborPolyhedron();
	if(neighbors[0])
		ids.push_back(neighbors[0]->getId());
	else
		ids.push_back(-1);
	if(neighbors[1])
		ids.push_back(neighbors[1]->getId());
	else
		ids.push_back(-1);

}
void PolygonUtils::getTriangleVertices(vis::Polygon* p,
									   std::vector<RVertexFlagAttribute>& flagAttributes){
	std::vector<vis::Vertex*> &vecVertices = p->getVertices();
	RVertexFlagAttribute virtualOEdge;
	virtualOEdge.enableFlag(RVertexFlagAttribute::VIRTUAL_OPOSITE_EDGE_FLAG);
	RVertexFlagAttribute realOEdge;
	if(p->isAtSurface()){
		virtualOEdge.enableFlag(RVertexFlagAttribute::SURFACE_POLYGON);
		realOEdge.enableFlag(RVertexFlagAttribute::SURFACE_POLYGON);
	}
	int verticesPos = flagAttributes.size();
	std::vector<glm::vec3>::size_type verticesPosStart = verticesPos;
	if(p->getVerticesCount() == 3){
		vecVertices[0]->addRmodelPositions(verticesPos++);
		vecVertices[1]->addRmodelPositions(verticesPos++);
		vecVertices[2]->addRmodelPositions(verticesPos++);
		PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
		flagAttributes.push_back(realOEdge);
		PolygonUtils::configRVertexFlagAttribute(vecVertices[1],realOEdge);
		flagAttributes.push_back(realOEdge);
		PolygonUtils::configRVertexFlagAttribute(vecVertices[2],realOEdge);
		flagAttributes.push_back(realOEdge);
	}
	else if(p->getVerticesCount() > 3){
		for( std::vector<vis::Vertex*>::size_type i = 1; i < vecVertices.size() - 1; i++ ) {
			vecVertices[0]->addRmodelPositions(verticesPos++);
			vecVertices[i]->addRmodelPositions(verticesPos++);
			vecVertices[i+1]->addRmodelPositions(verticesPos++);
			if(i==1){
				PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i+1],realOEdge);
				flagAttributes.push_back(realOEdge);
			}
			else if((vecVertices.size()-2) == i){
				PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i+1],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
			}
			else{
				PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i+1],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
			}
		}
	}
	for(std::vector<glm::vec3>::size_type i = verticesPosStart ;i < flagAttributes.size(); ++i )
		p->addRmodelPositions(verticesPosStart++);
	std::vector<int>& rmodelPos = p->getRmodelPositions();
	int nTriangles = p->getVerticesCount()-2;
	int mainTrianglePos = nTriangles/2*3;
	for(int i = 0;i<3;i++)
		flagAttributes[rmodelPos[mainTrianglePos+i]].
				enableFlag(RVertexFlagAttribute::REPRESENTS_POLYGON);
}

void PolygonUtils::getTriangleVertices(vis::LWPolygon* p,std::vector<glm::vec3>& vertices,
									   std::vector<glm::vec3>& normals,
									   std::vector<RVertexFlagAttribute>& flagAttributes,
									   std::vector<int>& ids,
									   std::vector<int>& vertexids){
	std::vector<vis::LWVertex*> &vecVertices = p->getVertices();
	glm::vec3 firstIndex = vecVertices[0]->getCoords();
	RVertexFlagAttribute virtualOEdge;
	virtualOEdge.enableFlag(RVertexFlagAttribute::VIRTUAL_OPOSITE_EDGE_FLAG);
	RVertexFlagAttribute realOEdge;
	if(p->isAtSurface()){
		virtualOEdge.enableFlag(RVertexFlagAttribute::SURFACE_POLYGON);
		realOEdge.enableFlag(RVertexFlagAttribute::SURFACE_POLYGON);
	}
	int verticesPos = vertices.size();
	std::vector<glm::vec3>::size_type verticesPosStart = verticesPos;
	if(p->getVerticesCount() == 3){
		vertices.push_back(firstIndex);
		normals.push_back(vecVertices[0]->getNormal());
		vertices.push_back(vecVertices[1]->getCoords());
		normals.push_back(vecVertices[1]->getNormal());
		vertices.push_back(vecVertices[2]->getCoords());
		normals.push_back(vecVertices[2]->getNormal());
		PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
		flagAttributes.push_back(realOEdge);
		PolygonUtils::configRVertexFlagAttribute(vecVertices[1],realOEdge);
		flagAttributes.push_back(realOEdge);
		PolygonUtils::configRVertexFlagAttribute(vecVertices[2],realOEdge);
		flagAttributes.push_back(realOEdge);
		addIds(p,ids);
		vertexids.push_back(vecVertices[0]->getId());
		vertexids.push_back(vecVertices[1]->getId());
		vertexids.push_back(vecVertices[2]->getId());

	}
	else if(p->getVerticesCount() > 3){
		for( std::vector<vis::Vertex*>::size_type i = 1; i < vecVertices.size() - 1; i++ ) {
			addIds(p,ids);
			vertexids.push_back(vecVertices[0]->getId());
			vertexids.push_back(vecVertices[i]->getId());
			vertexids.push_back(vecVertices[i+1]->getId());
			vertices.push_back(firstIndex);
			normals.push_back(vecVertices[0]->getNormal());
			vertices.push_back(vecVertices[i]->getCoords());
			normals.push_back(vecVertices[i]->getNormal());
			vertices.push_back(vecVertices[i+1]->getCoords());
			normals.push_back(vecVertices[i+1]->getNormal());
			if(i==1){
				PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i+1],realOEdge);
				flagAttributes.push_back(realOEdge);
			}
			else if((vecVertices.size()-2) == i){
				PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i+1],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
			}
			else{
				PolygonUtils::configRVertexFlagAttribute(vecVertices[0],realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
				PolygonUtils::configRVertexFlagAttribute(vecVertices[i+1],virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
			}
		}
	}
	int nTriangles = p->getVerticesCount()-2;
	int mainTrianglePos = nTriangles/2*3+verticesPosStart;//middle triangle
	for(int i = 0;i<3;i++)
		flagAttributes[mainTrianglePos+i].
				enableFlag(RVertexFlagAttribute::REPRESENTS_POLYGON);

}

float PolygonUtils::getDihedralAngle( vis::Polygon* p1,vis::Polygon* p2){
	//float dotPo = glm::dot(p1->getNormal(),-p2->getNormal());
	float dotPo = glm::dot(p1->getNormal(),p2->getNormal());
	if(dotPo<-1.0f)
		dotPo = -1.0f;
	else if(dotPo > 1.0f)
		dotPo = 1.0f;
	return std::acos(dotPo);
}
glm::vec3 getBiggestAnglePoint(glm::vec3& p1,
							   glm::vec3& p2,
							   std::vector<glm::vec3>& otherPoints){
	glm::vec3 lastvec = p2-p1;
	typedef std::vector<glm::vec3>::size_type s_type;
	s_type pos = 0;
	float maxAngle = -std::numeric_limits<float>::max();
	for(s_type i = 0;i<otherPoints.size();i++){
		glm::vec3 testVec = otherPoints[i]-p1;
		float angle = glm::dot(lastvec,testVec)/(glm::length(testVec)*glm::length(lastvec));
		if(angle>maxAngle){
			pos = i;
			maxAngle = angle;
		}
	}
	glm::vec3 chosen = otherPoints[pos];
	otherPoints[pos] = otherPoints[otherPoints.size()-1];
	otherPoints.pop_back();
	return chosen;
}
glm::vec3 getCenter(std::vector<glm::vec3>& vec){
	glm::vec3 center(0.0f,0.0f,0.0f);
	for(glm::vec3 pos : vec )
		center+=pos;
	center/=(float)vec.size();
	return center;
}

void PolygonUtils::orderVerticesToCCWPolygon(std::vector<glm::vec3>& vec){
	if(vec.size()==0u)
		return;
	std::vector<glm::vec3> orderedCCW;
	orderedCCW.push_back(getCenter(vec));//fake point, must be removed later
	orderedCCW.push_back(vec[vec.size()-1]);
	vec.pop_back();
	while(vec.size()>0u)
		orderedCCW.push_back(getBiggestAnglePoint(orderedCCW[orderedCCW.size()-2],orderedCCW[orderedCCW.size()-1],vec));
	orderedCCW[0] = orderedCCW[orderedCCW.size()-1];
	orderedCCW.pop_back();
	vec.swap(orderedCCW);

}
