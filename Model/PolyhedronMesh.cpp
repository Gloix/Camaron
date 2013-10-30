#include "Model/PolyhedronMesh.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Rendering/RModel/rmodel.h"
#include "ModelExport/modelexportstrategy.h"

PolyhedronMesh::PolyhedronMesh(std::string f):PolygonMesh(f)
{
	//ctor
	modelType = vis::CONSTANTS::POLYHEDRON_MESH;
}

PolyhedronMesh::~PolyhedronMesh()
{
	for(std::vector<vis::Polyhedron*>::size_type i = 0; i != polyhedrons.size(); i++)
		delete polyhedrons[i];
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
