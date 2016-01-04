#include "lightweightpolyhedronmesh.h"
#include "Rendering/RModel/rmodel.h"

LightWeightPolyhedronMesh::LightWeightPolyhedronMesh(std::string f):
	LightWeightPolygonMesh(f)
{
	//ctor
	modelType = vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH;
}

LightWeightPolyhedronMesh::~LightWeightPolyhedronMesh()
{
	for(vis::LWPolyhedron* lwPolyhedron : polyhedrons)
		delete lwPolyhedron;
}
int LightWeightPolyhedronMesh::getPolyhedronsCount(){
	return polyhedronsCount;
}

void LightWeightPolyhedronMesh::setPolyhedronsCount( int n){
	polyhedronsCount = n;
}
std::vector<vis::LWPolyhedron*>& LightWeightPolyhedronMesh::getPolyhedrons(){
	return polyhedrons;
}
void LightWeightPolyhedronMesh::clean(){
	for( vis::LWPolyhedron* lwPolyhedron : polyhedrons)
		delete lwPolyhedron;
	polyhedrons.clear();
	LightWeightPolygonMesh::clean();
}
DOUBLE_DISPATCH_MODEL_DEF_LIGHT_WEIGHT(LightWeightPolyhedronMesh)
