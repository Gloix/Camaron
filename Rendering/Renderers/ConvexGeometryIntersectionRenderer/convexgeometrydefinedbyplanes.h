#ifndef CONVEXGEOMETRYDEFINEDBYPLANES_H
#define CONVEXGEOMETRYDEFINEDBYPLANES_H
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/plane.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h"
#include <string>
class ConvexGeometryDefinedByPlanes:public GenericConvexGeometry
{
	public:
		ConvexGeometryDefinedByPlanes(ConvexGeometryIntersectionRendererConfig*);
		virtual ~ConvexGeometryDefinedByPlanes();
		void addPlane(Plane p);
		void calculateCorners();
		std::vector<glm::vec3>& getTriangleVertices();
		std::vector<std::vector<glm::vec3> >& getItersections();
		std::vector<Plane>& getOriginalPlanes();
		std::vector<Plane>& getMovedPlanes();
		void uploadMainVertexData();
		void setFromNormal(bool);

		//Generic
		void move(glm::mat4);
		virtual bool isPointInside(glm::vec3);
		virtual bool loadDataToGPU(glm::mat4);
		virtual void setUniformData(GLuint theProgram);
	private:
		bool fromNormals;
		void discardInsidePoints();
		std::vector<Plane> planes;
		std::vector<Plane> movedPlanes;
		std::vector<std::vector<glm::vec3> > planesIntersections;
		std::vector<glm::vec3> triangleVertices;

		GLuint planesDataTextureId;
};

#endif // CONVEXGEOMETRYDEFINEDBYPLANES_H
