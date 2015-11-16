#ifndef EDGE_H
#define EDGE_H
#include "Model/Element/Element.h"

namespace vis{
class Vertex;
class Edge : public vis::Element
{
	public:
		Edge(int idn, vis::Vertex* v0, vis::Vertex* v1);
		Edge(int idn, vis::Vertex* v0, vis::Vertex* v1, glm::vec3 color);
		vis::Vertex* getVertex0();
		vis::Vertex* getVertex1();
		void setColor(glm::vec3 color);
		glm::vec3& getColor();
		bool isAtSurface();
		glm::vec3 getGeometricCenter();
		DOUBLE_DISPATCH_ELEMENT_DEC
	private:

		vis::Vertex * const vertex0;
		vis::Vertex * const vertex1;
		glm::vec3 color;

};
}

#endif // EDGE_H
