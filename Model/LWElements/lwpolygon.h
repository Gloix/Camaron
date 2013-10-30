#ifndef LWPOLYGON_H
#define LWPOLYGON_H
#include "Model/LWElements/lwelement.h"
#include <vector>
#include <glm/glm.hpp>
namespace vis{
class LWVertex;
class LWPolyhedron;
class LWPolygon: public LWElement
{
	public:
		LWPolygon( int );
		virtual ~LWPolygon();
		std::vector<LWVertex*>& getVertices();
		int getVerticesCount();
		glm::vec3 getGeometricCenter();
		glm::vec3 getNormal();
		bool isAtSurface();
		void addPolyhedron(LWPolyhedron* pol);
		LWPolyhedron* getNeighborPolyhedron(LWPolyhedron* pol);
		LWPolyhedron** getNeighborPolyhedron();
		void invertVerticesOrder();
	protected:
		std::vector<LWVertex*> vertices;
		LWPolyhedron *polyhedrons[2];
};
}
#endif // LWPOLYGON_H
