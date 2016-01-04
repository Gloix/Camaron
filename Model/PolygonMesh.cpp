#include "Model/PolygonMesh.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include "Model/Element/Polygon.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Rendering/Renderer.h"
#include "Rendering/RModel/rmodel.h"
#include "ModelExport/modelexportstrategy.h"
#include "Model/modelvisitor.h"
#include <iostream>

PolygonMesh::PolygonMesh(std::string f,int nver, int npol):VertexCloud(f) {
	polygonsCount = npol;
	verticesCount = nver;
	modelType = vis::CONSTANTS::POLYGON_MESH;
}

PolygonMesh::~PolygonMesh()
{
	for(vis::Polygon* polygon : polygons)
		delete polygon;
}

int PolygonMesh::getPolygonsCount() {
	return this->polygonsCount;
}
void PolygonMesh::setPolygonsCount(int p) {
	this->polygonsCount = p;
}
std::vector<vis::Polygon*>& PolygonMesh::getPolygons() {
	return polygons;
}

DOUBLE_DISPATCH_MODEL_DEF(PolygonMesh)
