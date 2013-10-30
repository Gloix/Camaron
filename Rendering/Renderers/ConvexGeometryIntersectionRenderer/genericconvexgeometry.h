#ifndef GENERICCONVEXGEOMETRY_H
#define GENERICCONVEXGEOMETRY_H
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h"
class GenericConvexGeometry
{
	public:
		GenericConvexGeometry();
		GenericConvexGeometry(ConvexGeometryIntersectionRendererConfig*);
		virtual ~GenericConvexGeometry();
		virtual void move(glm::mat4) = 0;
		virtual bool isPointInside(glm::vec3) = 0;
		virtual void render(glm::mat4);
		virtual bool loadDataToGPU(glm::mat4) = 0;
		virtual void setUniformData(GLuint theProgram);
		static GLuint getGenericProgram();
	protected:
		static GLuint genericProgram;
		GLuint triangleVerticesBufferObject;
		GLuint normalsBufferObject;
		int nNormalsVertices;
		int nTrianglesVertices;
		static bool failed;
		ConvexGeometryIntersectionRendererConfig* config;
};

#endif // GENERICCONVEXGEOMETRY_H
