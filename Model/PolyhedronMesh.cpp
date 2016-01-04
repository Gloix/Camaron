#include "Model/PolyhedronMesh.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Rendering/RModel/rmodel.h"
#include "ModelExport/modelexportstrategy.h"
#include "Model/modelvisitor.h"

PolyhedronMesh::PolyhedronMesh(std::string f):PolygonMesh(f)
{
	//ctor
	modelType = vis::CONSTANTS::POLYHEDRON_MESH;
}

PolyhedronMesh::~PolyhedronMesh()
{
	for(vis::Polyhedron* polyhedron : polyhedrons)
		delete polyhedron;
}
int PolyhedronMesh::getPolyhedronsCount(){
	return polyhedronsCount;
}

void PolyhedronMesh::setPolyhedronsCount( int n){
	polyhedronsCount = n;
}
std::vector<vis::Polyhedron*>& PolyhedronMesh::getPolyhedrons(){
	return polyhedrons;
}
DOUBLE_DISPATCH_MODEL_DEF(PolyhedronMesh)
