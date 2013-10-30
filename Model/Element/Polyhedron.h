#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "Common/Constants.h"
#include <vector>
#include <glm/glm.hpp>
#include "Model/Element/Element.h"
namespace vis{
class Polygon;
class Polyhedron: public Element
{
	public:
		Polyhedron( int );
		virtual ~Polyhedron();
		std::vector<Polygon*>& getPolyhedronPolygons();
		glm::vec3 getGeometricCenter();
		int getPolygonsCount();
		void setGeoCenter(glm::vec3);
		void calculateGeoCenter();
		virtual float getArea();
		virtual float getVolume();
		bool isAtSurface();
		glm::vec3 getInwardNormal(vis::Polygon*);
		glm::vec3 getOutwardNormal(vis::Polygon*);

		DOUBLE_DISPATCH_ELEMENT_DEC
	protected:
		float volume;
		float area;
		std::vector<Polygon*> polygons;
		glm::vec3 geoCenter;
	private:
#ifdef USE_CONST_EXPR_FOR_STATIC_FLOAT
		static constexpr float NULL_VALUE = -1.0f;
#else
		static const float NULL_VALUE = -1.0f;
#endif
};
}
#endif // POLYHEDRON_H
