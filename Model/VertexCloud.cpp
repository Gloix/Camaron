#include "Model/VertexCloud.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include "Model/Element/Vertex.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Rendering/Renderer.h"
#include "Rendering/RModel/rmodel.h"
#include "ModelExport/modelexportstrategy.h"
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
	for( std::vector<vis::Vertex*>::size_type i = 0; i != vertices.size(); i++ )
		delete vertices[i];

    for( std::vector<VScalarDef*>::size_type i = 0; i != scalarDefs.size(); i++ )
        delete scalarDefs[i];
}

int VertexCloud::getVerticesCount() {
	return this->verticesCount;
}
void VertexCloud::setVerticesCount( int v ) {
	this->verticesCount = v;
}
std::vector<vis::Vertex*>& VertexCloud::getVertices() {
	return vertices;
}
bool VertexCloud::is2D(){
	return _2d;
}

void VertexCloud::set2D(bool b){
	_2d = b;
}

void VertexCloud::addScalarDef(VScalarDef* scalarDef) {
    scalarDefs.push_back(scalarDef);
}

std::vector<VScalarDef*> VertexCloud::getScalarDefs() {
    return scalarDefs;
}

DOUBLE_DISPATCH_MODEL_DEF(VertexCloud)
