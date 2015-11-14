#ifndef VERTEX_H
#define VERTEX_H
#include "Model/Element/Element.h"
#include <vector>
#include <glm/glm.hpp>

#define SCALAR_PROPERTY_START_INDEX 192

namespace vis{
class Polygon;
class Vertex: public Element {
	public:
		Vertex( int id = -1, float x = 0.0f, float y = 0.0f, float z = 0.0f, int pos = NULL_POS);
		virtual ~Vertex();
		glm::vec3& getCoords();
		glm::vec3 getGeometricCenter();
		glm::vec3& getNormal();
		void setNormal(glm::vec3);
		int getPos();
		void setPos(int);
		float getScalarProperty(int);
		void addScalarProperty(int, float);
		bool isAtSurface();
		std::vector<vis::Polygon*>& getVertexPolygons();
		static const int NULL_POS = -999;
		DOUBLE_DISPATCH_ELEMENT_DEC
	protected:
	private:
		glm::vec3 coords;
		glm::vec3 normal;
		int pos;//store it's position in the vertices array (not always the same as the id)
		std::vector<vis::Polygon*> polygons;
};
}
#endif // VERTEX_H
