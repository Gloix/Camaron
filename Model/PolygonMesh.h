#ifndef POLYGONMESH_H
#define POLYGONMESH_H

#include <vector>
#include <map>
#include <glm/glm.hpp>
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
		// scalarPointsInPolygon holds the points' coordinates in barycentric coordinates
		std::map<unsigned char, std::vector<glm::vec3>> scalarPointsInPolygon;
		int polygonsCount;
		std::vector<vis::Polygon*> polygons;
};

#endif // POLYGONMESH_H
