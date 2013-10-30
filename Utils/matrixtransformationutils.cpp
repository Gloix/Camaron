#include "matrixtransformationutils.h"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <iostream>
#include "Utils/Printer.h"
MatrixTransformationUtils::MatrixTransformationUtils()
{
}
glm::mat4 MatrixTransformationUtils::generateRotationMatrix(glm::vec3 rotation){
	return MatrixTransformationUtils::generateXRotationMatrix(rotation.x)*
			MatrixTransformationUtils::generateYRotationMatrix(rotation.y)*
			MatrixTransformationUtils::generateZRotationMatrix(rotation.z);
}

void MatrixTransformationUtils::setUpRotationMatrix(glm::mat4& m,float xRot,float yRot,float zRot){
	glm::mat4 mat = MatrixTransformationUtils::generateRotationMatrix(glm::vec3(xRot,yRot,zRot));
	for(int i = 0;i<4;i++)
		for(int j = 0;j<4;j++)
			m[i][j] = mat[i][j];
}

glm::mat4 MatrixTransformationUtils::generateXRotationMatrix(float xRot){
	glm::mat4 matrix;
	float sinxRot = std::sin(xRot);
	float cosxRot = std::cos(xRot);
	matrix[1].y = cosxRot;
	matrix[1].z = sinxRot;
	matrix[2].y = -sinxRot;
	matrix[2].z = cosxRot;
	return matrix;
	//return glm::rotate(glm::mat4(1.0f), xRot, glm::vec3(1.0f,0.0f,0.0f));
}

glm::mat4 MatrixTransformationUtils::generateYRotationMatrix(float yRot){
	glm::mat4 matrix;
	float sinxRot = std::sin(yRot);
	float cosxRot = std::cos(yRot);
	matrix[0].x = cosxRot;
	matrix[0].z = -sinxRot;
	matrix[2].x = sinxRot;
	matrix[2].z = cosxRot;
	return matrix;
	//return glm::rotate(glm::mat4(1.0f), xRot, glm::vec3(1.0f,0.0f,0.0f));
}

glm::mat4 MatrixTransformationUtils::generateZRotationMatrix(float zRot){
	glm::mat4 matrix;
	float sinxRot = std::sin(zRot);
	float cosxRot = std::cos(zRot);
	matrix[0].x = cosxRot;
	matrix[0].y = sinxRot;
	matrix[1].x = -sinxRot;
	matrix[1].y = cosxRot;
	return matrix;
}
glm::mat4 MatrixTransformationUtils::generateScaleMatrix(float scale){
	glm::mat4 matrix;
	matrix[0].x = scale;
	matrix[1].y = scale;
	matrix[2].z = scale;
	return matrix;
}
glm::mat4 MatrixTransformationUtils::generateTranslationMatrix(glm::vec3 traslation){
	glm::mat4 translationMatrix;
	translationMatrix[3].x = -traslation.x;
	translationMatrix[3].y = -traslation.y;
	translationMatrix[3].z = -traslation.z;
	return translationMatrix;
}
glm::mat4 MatrixTransformationUtils::generateViewPortMatrix(int x, int y, int w, int h, float n, float f){
	glm::mat4 viewport;
	viewport[0].x = w/2.0f;
	viewport[1].y = h/2.0f;
	viewport[2].z = (f-n)/2.0f;

	viewport[3].x = x + w/2.0f;
	viewport[3].y = y + h/2.0f;
	viewport[3].z = (f+n)/2.0f;
	return viewport;
}
glm::mat4 MatrixTransformationUtils::generateOrthoProjectionMatrix(float left, float right, float top, float bottom, float far, float near){
	glm::mat4 proj;
	proj[0].x = 2.0f/(right-left);
	proj[1].y = 2.0f/(top-bottom);
	proj[2].z = -2.0f/(far-near);

	proj[3].x = -(right+left)/(right-left);
	proj[3].y = -(top+bottom)/(top-bottom);
	proj[3].z = -(far+near)/(far-near);
	return proj;
}
glm::mat4 MatrixTransformationUtils::generateModelToCameraSpaceMatrix(std::vector<float>& bounds){
	//{min x, min y, min z, max x, max y, max z}
	if(bounds.size()<6)
		return glm::mat4();
	float xCenter = (bounds[0]+bounds[3])/2.0f;
	float yCenter = (bounds[1]+bounds[4])/2.0f;
	float zCenter = (bounds[2]+bounds[5])/2.0f;

	float maxBound = 1.0f;//keep scale
	maxBound = std::max(maxBound, std::abs(bounds[0] - xCenter));
	maxBound = std::max(maxBound, std::abs(bounds[1] - yCenter));
	maxBound = std::max(maxBound, std::abs(bounds[2] - zCenter));
	maxBound = std::max(maxBound, std::abs(bounds[3] - xCenter));
	maxBound = std::max(maxBound, std::abs(bounds[4] - yCenter));
	maxBound = std::max(maxBound, std::abs(bounds[5] - zCenter));
	glm::mat4 scaleMatrix = MatrixTransformationUtils::generateScaleMatrix(1.0f/maxBound);
	//std::cout << "Scale Matrix: " << std::endl;
	//Printer::print(scaleMatrix);
	//glm::mat4 translationMatrix = MatrixTransformationUtils::generateTranslationMatrix(xCenter, yCenter, zCenter);

	//std::cout << "Scale*Translation Matrix: " << std::endl;
	//glm::mat4 com = scaleMatrix*translationMatrix;
	//Printer::print(com);
	return scaleMatrix * MatrixTransformationUtils::generateTranslationMatrix(glm::vec3(xCenter,yCenter,zCenter));
}
glm::mat4 MatrixTransformationUtils::generatePerspectiveProjectionMatrix(float frustumScaleX, float frustumScaleY,
																		 float far, float near,
																		 float top, float bottom,
																		 float right, float left){
	glm::mat4 proj;
	proj[0].x = frustumScaleX;
	proj[1].y = frustumScaleY;
	proj[2].w = -1.0f;
	proj[2].z = -far/(far-near);
	proj[3].z = far*near/(near-far);
	//
	proj[2].x = (right+left)/(right-left);
	proj[2].y = (top+bottom)/(top-bottom);
	return proj;
}
float MatrixTransformationUtils::CalcFrustumScale(float fFovDeg){
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	float fFovRad = fFovDeg * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
}
