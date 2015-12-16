#ifndef LIGHTWEIGHTPOLYHEDRONMESH_H
#define LIGHTWEIGHTPOLYHEDRONMESH_H
#include "Model/lightweightpolygonmesh.h"
#include "Model/LWElements/lwpolyhedron.h"
#include "Model/modelvisitor.h"
class LightWeightPolyhedronMesh: public LightWeightPolygonMesh
{
	public:
		LightWeightPolyhedronMesh(std::string);
		virtual ~LightWeightPolyhedronMesh();
		std::vector<vis::LWPolyhedron*>& getPolyhedrons();
		int getPolyhedronsCount();
		void setPolyhedronsCount( int );
		virtual void clean();
		DOUBLE_DISPATCH_MODEL_DEC
	protected:
	private:
		int polyhedronsCount;
		std::vector<vis::LWPolyhedron*> polyhedrons;
};

#endif // LIGHTWEIGHTPOLYHEDRONMESH_H
