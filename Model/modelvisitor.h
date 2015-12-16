#ifndef RENDERERCONFIGVISITOR_H
#define RENDERERCONFIGVISITOR_H

class Model;
class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class LightWeightVertexCloud;
class LightWeightPolygonMesh;
class LightWeightPolyhedronMesh;

class ModelVisitor
{
	public:
		virtual void visit(Model*) {}
		virtual void visit(VertexCloud*) {}
		virtual void visit(PolygonMesh*) {}
		virtual void visit(PolyhedronMesh*) {}
		virtual void visit(LightWeightVertexCloud*) {}
		virtual void visit(LightWeightPolygonMesh*) {}
		virtual void visit(LightWeightPolyhedronMesh*) {}
};

#endif // RENDERERCONFIGVISITOR_H
