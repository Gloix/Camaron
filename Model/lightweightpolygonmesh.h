#ifndef LIGHTWEIGHTPOLYGONMESH_H
#define LIGHTWEIGHTPOLYGONMESH_H
#include "Model/lightweightvertexcloud.h"
#include "Model/LWElements/lwpolygon.h"
class LightWeightPolygonMesh: public LightWeightVertexCloud
{
	public:
		LightWeightPolygonMesh(std::string, int nver = 0, int npol = 0);
		virtual ~LightWeightPolygonMesh();
		std::vector<vis::LWPolygon*>& getPolygons();
		int getPolygonsCount();
		void setPolygonsCount( int );
		DOUBLE_DISPATCH_MODEL_DEC
		virtual void clean();
	protected:
	private:
		int polygonsCount;
		std::vector<vis::LWPolygon*> polygons;
};

#endif // LIGHTWEIGHTPOLYGONMESH_H
