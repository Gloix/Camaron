#ifndef MATRIXTRANSFORMATIONUTILS_H
#define MATRIXTRANSFORMATIONUTILS_H
#include <glm/glm.hpp>
#include <vector>
class MatrixTransformationUtils
{
	public:
		static glm::mat4 generateRotationMatrix(glm::vec3);
		static glm::mat4 generateXRotationMatrix(float);
		static glm::mat4 generateYRotationMatrix(float);
		static glm::mat4 generateZRotationMatrix(float);
		static glm::mat4 generateScaleMatrix(float);
		static glm::mat4 generateTranslationMatrix(glm::vec3);
		static glm::mat4 generateModelToCameraSpaceMatrix(std::vector<float>& bounds);
		static glm::mat4 generateViewPortMatrix(int x, int y, int w, int h, float n, float f);
		static glm::mat4 generateOrthoProjectionMatrix(float left, float right, float top, float bottom, float far, float near);
		static glm::mat4 generatePerspectiveProjectionMatrix(float frustumScaleX, float frustumScaleY,float far, float near,float top, float bottom, float right, float left);
		static void setUpRotationMatrix(glm::mat4&,float xRot,float yRot,float zRot);
		static float CalcFrustumScale(float fFovDeg);
	private:
		MatrixTransformationUtils();
};

#endif // MATRIXTRANSFORMATIONUTILS_H
