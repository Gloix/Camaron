#ifndef POLYHEDRONUTILS_H
#define POLYHEDRONUTILS_H

#include <vector>
#include <glm/glm.hpp>
namespace vis {
class Polyhedron;
class Vertex;
}
class PolyhedronUtils
{
	public:
		static void getPolyhedronVertices(vis::Polyhedron*, std::vector<vis::Vertex*>&);
		static void setPolyhedronRModelPositions(vis::Polyhedron*);
		static float getPolyhedronSolidAngleFromVertex(vis::Polyhedron* p,
													   vis::Vertex* v);
	private:
		PolyhedronUtils();
		static glm::vec3 getCoordsSphericalCoordinates(glm::vec3);
};

#endif // POLYHEDRONUTILS_H
