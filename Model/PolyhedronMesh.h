#ifndef POLYHEDRONMESH_H
#define POLYHEDRONMESH_H

#include "Model/Element/Polyhedron.h"
#include "Model/PolygonMesh.h"

class ModelVisitor;

class PolyhedronMesh: public PolygonMesh
{
	public:
		PolyhedronMesh(std::string);
		virtual ~PolyhedronMesh();
		std::vector<vis::Polyhedron*>& getPolyhedrons();
		int getPolyhedronsCount();
		void setPolyhedronsCount( int );

		DOUBLE_DISPATCH_MODEL_DEC
	protected:
	private:
		int polyhedronsCount;
		std::vector<vis::Polyhedron*> polyhedrons;
};

#endif // POLYHEDRONMESH_H
