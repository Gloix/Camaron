#include "sphericconvexgeometry.h"
#include "Common/Constants.h"
#include "Utils/shaderutils.h"
SphericConvexGeometry::SphericConvexGeometry(ConvexGeometryIntersectionRendererConfig *c):
	GenericConvexGeometry(c)
{
}

void SphericConvexGeometry::setSphereAttributes(float f, glm::vec3 v){
	center = v;
	radio = f;
	movedRadio = f;
	movedCenter = v;
}
void SphericConvexGeometry::createVertexData(int nPress){
	float thetaMax = PI;
	float thetaMin = 0;
	float phiMax = 2*PI;
	float phiMin = 0;

	int nSquares = nPress/2;
	int squarenSquare = (int)std::sqrt(nSquares);
	float thetaStep = (thetaMax-thetaMin)/squarenSquare;
	float phiStep = (phiMax-phiMin)/squarenSquare;
	std::vector<glm::vec3> triangles;
	float thetaitNext;
	for(float thetait = thetaMin;thetait < thetaMax; thetait = thetaitNext){
		thetaitNext = std::min(thetaMax,thetaStep+thetait);
		float phiitNext;
		for(float phiit = phiMin;phiit < phiMax; phiit = phiitNext){
			phiitNext = std::min(phiMax,phiStep+phiit);
			glm::vec3 p1 = center+glm::vec3(radio*std::sin(thetait)*cos(phiit),
					  radio*std::sin(thetait)*sin(phiit),
					  radio*std::cos(thetait));
			glm::vec3 p2 = center+glm::vec3(radio*std::sin(thetaitNext)*cos(phiit),
					  radio*std::sin(thetaitNext)*sin(phiit),
					  radio*std::cos(thetaitNext));
			glm::vec3 p3 = center+glm::vec3(radio*std::sin(thetait)*cos(phiitNext),
					  radio*std::sin(thetait)*sin(phiitNext),
					  radio*std::cos(thetait));
			glm::vec3 p4 = center+glm::vec3(radio*std::sin(thetaitNext)*cos(phiitNext),
					  radio*std::sin(thetaitNext)*sin(phiitNext),
					  radio*std::cos(thetaitNext));
			triangles.push_back(p1);
			triangles.push_back(p2);
			triangles.push_back(p3);

			triangles.push_back(p2);
			triangles.push_back(p4);
			triangles.push_back(p3);

		}
	}
	triangleVerticesBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(triangles);
	nTrianglesVertices = triangles.size();

}
void SphericConvexGeometry::move(glm::mat4 mat){
	movedCenter = glm::vec3(mat*glm::vec4(center,1.0f));
	glm::vec4 raPoint = glm::vec4(center,1.0f) + glm::vec4(radio,0.0f,0.0f,0.0f);
	raPoint = mat*raPoint;
	raPoint/=raPoint.w;
	movedRadio = glm::distance(movedCenter,glm::vec3(raPoint));
}

bool SphericConvexGeometry::isPointInside(glm::vec3 vec){
	return glm::distance(vec,movedCenter)<movedRadio;
}


bool SphericConvexGeometry::loadDataToGPU(glm::mat4 mat){
	move(mat);
	return true;
}

void SphericConvexGeometry::setUniformData(GLuint theProgram){
	ShaderUtils::setUniform(theProgram,"isSphere",true);
	ShaderUtils::setUniform(theProgram,"sphereRadio",movedRadio);
	ShaderUtils::setUniform(theProgram,"sphereCenter",movedCenter);
}
