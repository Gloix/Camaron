#include "convexgeometrydefinedbyplanes.h"
#include "Utils/PolygonUtils.h"
#include "Utils/chararrayscanner.h"
#include "Utils/shaderutils.h"
#include "Utils/openglutils.h"
#include <cstring>

#define POSITION_ATTRIBUTE 1
ConvexGeometryDefinedByPlanes::ConvexGeometryDefinedByPlanes(ConvexGeometryIntersectionRendererConfig* c):
	GenericConvexGeometry(c)
{
	planesDataTextureId = 0;
}
ConvexGeometryDefinedByPlanes::~ConvexGeometryDefinedByPlanes(){
}

void ConvexGeometryDefinedByPlanes::addPlane(Plane p){
	planes.push_back(p);
}

void ConvexGeometryDefinedByPlanes::uploadMainVertexData(){
	triangleVerticesBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(triangleVertices);
	if(triangleVerticesBufferObject)
		nTrianglesVertices = triangleVertices.size();
}

void ConvexGeometryDefinedByPlanes::calculateCorners(){
	for(std::vector<Plane>::size_type p = 0;p<planes.size();p++){
		std::vector<glm::vec3> currentPlaneCorners;
		for(std::vector<Plane>::size_type p2 = 0;p2<planes.size();p2++){
			if(p==p2)
				continue;
			for(std::vector<Plane>::size_type p3 = 0;p3<planes.size();p3++){
				if(p==p3 || p3 == p2)
					continue;
				if(planes[p].intersect(planes[p2],planes[p3]))
					currentPlaneCorners.push_back(planes[p].getPointOfIntersection(planes[p2],planes[p3]));
			}
		}
		if(currentPlaneCorners.size())
			planesIntersections.push_back(currentPlaneCorners);
	}
	//discardInsidePoints();
	for(std::vector<std::vector<glm::vec3> >::size_type i = 0;i<planesIntersections.size();i++)
		PolygonUtils::orderVerticesToCCWPolygon(planesIntersections[i]);
	if(fromNormals){
		//do
		for(std::vector<std::vector<glm::vec3> >::size_type i = 0;i<planesIntersections.size();i++){
			std::vector<glm::vec3>& currentPol = planesIntersections[i];
			for(std::vector<glm::vec3>::size_type j = 1;j<currentPol.size()-1;j++){
				triangleVertices.push_back(currentPol[0]);
				triangleVertices.push_back(currentPol[j]);
				triangleVertices.push_back(currentPol[j+1]);
			}
		}

	}
}
std::vector<std::vector<glm::vec3> >& ConvexGeometryDefinedByPlanes::getItersections(){
	return planesIntersections;
}

void ConvexGeometryDefinedByPlanes::discardInsidePoints(){
	for(std::vector<std::vector<glm::vec3> >::size_type i = 0;i<planesIntersections.size();i++){
		std::vector<glm::vec3>& points = planesIntersections[i];
		for(std::vector<glm::vec3>::size_type j = 0;j<points.size();j++){
			for(std::vector<Plane>::size_type p = 0;p<planes.size();p++){
				if(!planes[p].isLeft(points[j])){
					points[j] = points[points.size()-1];
					points.pop_back();
					j--;;//move back j, to check same position again
					if(!points.size()){
						j = points.size();//end j
						planesIntersections[i] = planesIntersections[planesIntersections.size()-1];
						planesIntersections.pop_back();
						i--;//move back i
					}
					p = planes.size();//end p
				}
			}
		}
	}
}
std::vector<Plane>& ConvexGeometryDefinedByPlanes::getOriginalPlanes(){
	return planes;
}
std::vector<Plane>& ConvexGeometryDefinedByPlanes::getMovedPlanes(){
	if(movedPlanes.size()!=planes.size()){
		movedPlanes.clear();
		for(std::vector<Plane>::size_type i = 0;i<planes.size();i++)
			movedPlanes.push_back(planes[i]);
	}
	return movedPlanes;
}

void ConvexGeometryDefinedByPlanes::move(glm::mat4 matrix){
	movedPlanes.clear();
	for(std::vector<Plane>::size_type i = 0;i<planes.size();i++)
		movedPlanes.push_back(Plane(glm::vec3(matrix*glm::vec4(planes[i].getOrigin(),1.0f)),
									glm::mat3(matrix)*planes[i].getNormal()));

}

bool ConvexGeometryDefinedByPlanes::isPointInside(glm::vec3 vec){
	for(std::vector<Plane>::size_type j = 0;j<movedPlanes.size();j++){
		if(!movedPlanes[j].isLeft(vec)){
			return false;
		}
	}
	return true;
}

bool ConvexGeometryDefinedByPlanes::loadDataToGPU(glm::mat4 matrix){
	//upload normal data
	std::vector<glm::vec3> normals;
	for(std::vector<Plane>::size_type i = 0;i<planes.size();i++){
		Plane& p = planes[i];
		normals.push_back(p.getOrigin());
		normals.push_back(p.getOrigin()+p.getNormal()*config->normalLength);
	}
	normalsBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(normals);
	if(normalsBufferObject)
		nNormalsVertices = normals.size();
	if(triangleVerticesBufferObject && planes.size()){
		OpenGLUtils::deleteTexture(planesDataTextureId);
		std::vector<glm::vec4> planeTextureData;
		for(std::vector<Plane>::size_type it = 0;it<planes.size();++it){
			Plane& p = planes[it];
			planeTextureData.push_back(glm::vec4(glm::normalize(glm::mat3(matrix)*p.getNormal()),1.0f));
			planeTextureData.push_back(matrix*glm::vec4(p.getOrigin(),1.0f));
		}
		planesDataTextureId = OpenGLUtils::uploadRaw1DTexture((unsigned char*)&planeTextureData[0],
															  planes.size()*2u,
															  GL_RGBA32F,GL_RGBA,GL_FLOAT);
		if(!planesDataTextureId)
			return false;
		glEnable(GL_TEXTURE_1D);
		glBindTexture(GL_TEXTURE_1D, planesDataTextureId);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,
						GL_NEAREST);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,
						GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glBindTexture(GL_TEXTURE_1D, 0);
		glDisable(GL_TEXTURE_1D);
		return true;
	}
	return false;
}
void ConvexGeometryDefinedByPlanes::setFromNormal(bool b){
	fromNormals = b;
}

void ConvexGeometryDefinedByPlanes::setUniformData(GLuint theProgram){
	glEnable(GL_TEXTURE_1D);
	glActiveTexture(GL_TEXTURE0);
	ShaderUtils::setUniformTexture(theProgram,"Planes",GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, planesDataTextureId);
	ShaderUtils::setUniform(theProgram, "planesModes",true);
	ShaderUtils::setUniform(theProgram, "nPlanes",(int)planes.size());
}

std::vector<glm::vec3>& ConvexGeometryDefinedByPlanes::getTriangleVertices(){
	return triangleVertices;
}
