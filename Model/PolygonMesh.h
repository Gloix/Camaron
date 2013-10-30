#ifndef POLYGONMESH_H
#define POLYGONMESH_H

#include <vector>
#include "Model/VertexCloud.h"
namespace vis{
class Polygon;
}
class PolygonMesh: public VertexCloud
{
	public:
		PolygonMesh( std::string, int nver = 0, int npol = 0);
		virtual ~PolygonMesh();
		std::vector<vis::Polygon*>& getPolygons();
		int getPolygonsCount();
		void setPolygonsCount( int );
		DOUBLE_DISPATCH_MODEL_DEC

	protected:
	private:
		int polygonsCount;
		std::vector<vis::Polygon*> polygons;
};

#endif // POLYGONMESH_H
