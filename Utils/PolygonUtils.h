#ifndef POLYGONUTILS_H
#define POLYGONUTILS_H

#include "Model/Element/Polygon.h"
#include "Model/LWElements/lwpolygon.h"
#include <vector>
#include "GL/glew.h"
#include <glm/glm.hpp>
class RVertexFlagAttribute;
class PolygonUtils
{
	public:
		static float getDihedralAngle( vis::Polygon*,vis::Polygon* );
		static void getTriangleIndices(vis::Polygon*, std::vector<GLuint>&);
		static void getTriangleVertices(vis::LWPolygon *, std::vector<glm::vec3>&,
										std::vector<glm::vec3>& normals,
										std::vector<RVertexFlagAttribute>& flagAttributes,
										std::vector<int>&,
										std::vector<int>& vertexids);
		static void getTriangleVertices(vis::Polygon*,
										std::vector<RVertexFlagAttribute>& flagAttributes);
		static void orderVerticesToCCWPolygon(std::vector<glm::vec3>&);
	protected:
	private:
		static void configRVertexFlagAttribute(vis::LWVertex* element, RVertexFlagAttribute& attribute);
		static void configRVertexFlagAttribute(vis::Element* element, RVertexFlagAttribute& attribute);
		PolygonUtils();
		virtual ~PolygonUtils();
};

#endif // POLYGONUTILS_H
