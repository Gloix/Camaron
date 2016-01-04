#include "lightweightpolygonmesh.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/modelvisitor.h"

LightWeightPolygonMesh::LightWeightPolygonMesh(std::string s,
											   int nver,
											   int npol):
	LightWeightVertexCloud(s)
{
	polygonsCount = npol;
	verticesCount = nver;
	modelType = vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH;
}

LightWeightPolygonMesh::~LightWeightPolygonMesh()
{
	for(vis::LWPolygon* lwPolygon : polygons)
		delete lwPolygon;
}

int LightWeightPolygonMesh::getPolygonsCount() {
	return this->polygonsCount;
}
void LightWeightPolygonMesh::setPolygonsCount(int p) {
	this->polygonsCount = p;
}
std::vector<vis::LWPolygon*>& LightWeightPolygonMesh::getPolygons() {
	return polygons;
}
void LightWeightPolygonMesh::clean(){
	for(vis::LWPolygon* lwPolygon : polygons)
		delete lwPolygon;
	polygons.clear();
	LightWeightVertexCloud::clean();
}

DOUBLE_DISPATCH_MODEL_DEF_LIGHT_WEIGHT(LightWeightPolygonMesh)
