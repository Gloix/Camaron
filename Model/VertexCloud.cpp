#include "Model/VertexCloud.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Edge.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Rendering/Renderer.h"
#include "Rendering/RModel/rmodel.h"
#include "ModelExport/modelexportstrategy.h"
#include "Model/modelvisitor.h"
#include <iostream>

VertexCloud::VertexCloud(std::string f):
	Model(f)
{
	_2d = false;
	//ctor
	modelType = vis::CONSTANTS::VERTEX_CLOUD;
}

VertexCloud::~VertexCloud()
{
	for( vis::Vertex* vertex : vertices )
		delete vertex;

	for( vis::Edge* edge : additionalEdges )
		delete edge;
}

int VertexCloud::getVerticesCount() {
	return this->verticesCount;
}
int VertexCloud::getAdditionalEdgesCount() {
	return this->additionalEdgesCount;
}
void VertexCloud::setVerticesCount( int v ) {
	this->verticesCount = v;
}
void VertexCloud::setAdditionalEdgesCount( int e ) {
	this->additionalEdgesCount = e;
}
std::vector<vis::Vertex*>& VertexCloud::getVertices() {
	return vertices;
}
std::vector<vis::Edge*>& VertexCloud::getAdditionalEdges() {
	return additionalEdges;
}
bool VertexCloud::is2D(){
	return _2d;
}

void VertexCloud::set2D(bool b){
	_2d = b;
}

DOUBLE_DISPATCH_MODEL_DEF(VertexCloud)
