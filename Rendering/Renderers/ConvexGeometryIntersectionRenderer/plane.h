#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>
class Plane
{
	public:
		Plane(glm::vec3 planeEq, float nd);
		Plane(glm::vec3 norigin, glm::vec3 v1,glm::vec3 v2);
		Plane(glm::vec3 norigin, glm::vec3 nnormal);
		Plane(float a,float b, float c, float d);
		virtual ~Plane();
		glm::vec3 getPointOfIntersection(Plane&,Plane&);
		glm::vec3 getPlaneEquation();
		glm::vec3 getNormal();
		glm::vec3 getOrigin();
		float getPlaneEquationConstant();
		bool intersect(Plane&,Plane&);
		void fixNormal(glm::vec3);
		bool isOk();
		bool isLeft(glm::vec3);
	private:
		glm::vec3 normal;
		glm::vec3 origin;
		//a*x + b*y + c*z = d
		glm::vec3 planeEquation; // a,b,c
		float d;// d
		bool ok;
};

#endif // PLANE_H
