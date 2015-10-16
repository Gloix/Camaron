#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <stdarg.h>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
struct ShaderLoadingData{
	public:
		ShaderLoadingData(GLenum t){eShaderType = t;}
		void addFile(std::string t){strShaderFiles.push_back(t);}
		GLenum eShaderType;
		std::vector<std::string> strShaderFiles;
};
struct VertexAttributeBindingData{
	public:
		GLuint index;
		const char* strShaderFile;
};
struct TransformFeedbackData{
    public:
        const std::vector<std::string> varyings;
        GLenum bufferMode;
};
class ShaderUtils
{
	public:
		static const GLuint FAIL_CREATING_SHADER = 99999;
		static const GLuint FAIL_CREATING_PROGRAM = 99998;
		static GLuint CreateProgram(const std::vector<GLuint> &shaderList,
									const std::vector<VertexAttributeBindingData> &attributes,
                                    const std::vector<VertexAttributeBindingData> &fragData,
                                    const TransformFeedbackData *transformFeedbackVaryings);
		static GLuint CreateProgram(const std::vector<ShaderLoadingData> &shaderList);
		static GLuint CreateProgram(const std::vector<ShaderLoadingData> &shaderList,
									const std::vector<VertexAttributeBindingData> &attributes);
		static GLuint CreateProgram(const std::vector<ShaderLoadingData> &shaderList,
									const std::vector<VertexAttributeBindingData> &attributes,
									const std::vector<VertexAttributeBindingData> &fragAt);
        static GLuint CreateProgram(const std::vector<ShaderLoadingData> &shaderList,
                                    const std::vector<VertexAttributeBindingData> &attributes,
                                    const TransformFeedbackData *transformFeedbackVaryings);
        static GLuint CreateProgram(const std::vector<ShaderLoadingData> &shaderList,
                                    const std::vector<VertexAttributeBindingData> &attributes,
                                    const std::vector<VertexAttributeBindingData> &fragAt,
                                    const TransformFeedbackData *transformFeedbackVaryings);
		static GLuint CreateShader(ShaderLoadingData data);

		//set uniforms
		static bool setUniform(GLuint program,const char* uniformName, glm::vec4 val);
		static bool setUniform(GLuint program,const char* uniformName, glm::mat4 val);
		static bool setUniform(GLuint program,const char* uniformName, glm::mat3 val);
		static bool setUniform(GLuint program,const char* uniformName, glm::vec3 val);
		static bool setUniform(GLuint program,const char* uniformName, glm::vec2 val);
		static bool setUniform(GLuint program,const char* uniformName, int val);
		static bool setUniform(GLuint program,const char* uniformName, float val);
		static bool setUniform(GLuint program,const char* uniformName, bool val);
        static bool setUniform(GLuint program,const char* uniformName, std::vector<float> val);
		static bool setUniformTexture(GLuint program,const char* uniformName, GLuint val);

		//buffers
		static void deleteBuffer(GLuint*);
		template <class myType>
		static GLuint createDataBuffer(std::vector<myType>&);
		template <class myType>
		static GLuint createDataBuffer(myType*,int n);
		template <class myType>
		static bool setDataBuffer(GLuint bufferHandle,std::vector<myType>&);
	private:
		ShaderUtils();
};

template <class myType>
GLuint ShaderUtils::createDataBuffer(std::vector<myType>& vector){
	GLuint handle;
	glGenBuffers(1, &handle);
	if(!handle)
		return 0;
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myType)*vector.size(), &vector[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return handle;
}
template <class myType>
GLuint ShaderUtils::createDataBuffer(myType* vector,int n){
	GLuint handle;
	glGenBuffers(1, &handle);
	if(!handle)
		return 0;
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myType)*n, vector, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return handle;
}

template <class myType>
bool ShaderUtils::setDataBuffer(GLuint bufferHandle,std::vector<myType>& vector){
	if(!bufferHandle)
		return false;
	glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myType)*vector.size(), &vector[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

#endif // SHADERUTILS_H
