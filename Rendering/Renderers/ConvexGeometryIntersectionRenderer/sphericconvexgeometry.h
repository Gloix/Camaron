#ifndef SPHERICCONVEXGEOMETRY_H
#define SPHERICCONVEXGEOMETRY_H

#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h"
class SphericConvexGeometry:public GenericConvexGeometry
{
	public:
		SphericConvexGeometry(ConvexGeometryIntersectionRendererConfig*);
		//Generic
		void setSphereAttributes(float, glm::vec3);
		void createVertexData(int);
		virtual void move(glm::mat4);
		virtual bool isPointInside(glm::vec3);
		virtual bool loadDataToGPU(glm::mat4);
		virtual void setUniformData(GLuint theProgram);
	private:
		float radio;
		float movedRadio;
		glm::vec3 center;
		glm::vec3 movedCenter;
};

#endif // SPHERICCONVEXGEOMETRY_H
