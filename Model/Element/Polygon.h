#ifndef POLYGON_H
#define POLYGON_H

#include <glm/glm.hpp>
#include "Model/Element/Element.h"
#include <vector>

#define NULL_AREA -1.0f

namespace vis{
class Vertex;
class Polyhedron;
class Polygon: public vis::Element
{
	public:
		Polygon( int );
		virtual ~Polygon();
		std::vector<Vertex*>& getVertices();
		std::vector<Polygon *> &getNeighborPolygons();
		glm::vec3 getGeometricCenter();
		int getVerticesCount();
		virtual bool isNeighbor(Polygon* p);
		glm::vec3& getNormal();
		void calculateNormal();
		virtual float getArea();
		bool isAtSurface();
		void addPolyhedron(Polyhedron* pol);
		Polyhedron* getNeighborPolyhedron(Polyhedron* pol);
		Polyhedron** getNeighborPolyhedron();
		virtual void invertVerticesOrder();

		DOUBLE_DISPATCH_ELEMENT_DEC
	protected:
		float area;
		std::vector<Vertex*> vertices;
		std::vector<Polygon*> neighborPolygons;
		glm::vec3 normal;//if it is a surface polygon, it should point out.

		//polyhedron
		Polyhedron *polyhedrons[2];
	private:
};
}
#endif // POLYGON_H
