#ifndef LWPOLYHEDRON_H
#define LWPOLYHEDRON_H
#include "lwelement.h"
#include <vector>
#include <glm/glm.hpp>
namespace vis{
class LWPolygon;
class LWPolyhedron: public LWElement
{
	public:
		LWPolyhedron( int );
		virtual ~LWPolyhedron();
		std::vector<LWPolygon*>& getPolyhedronPolygons();
		int getPolygonsCount();
		glm::vec3 getGeometricCenter();
		bool isAtSurface();
		glm::vec3 getInwardNormal(vis::LWPolygon*);
		glm::vec3 getOutwardNormal(vis::LWPolygon*);

	protected:
		std::vector<LWPolygon*> polygons;
};
}
#endif // LWPOLYHEDRON_H
