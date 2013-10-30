#include "shaderutils.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <glm/ext.hpp>
#include <QFile>
#include <QTextStream>
#include <Common/Constants.h>
ShaderUtils::ShaderUtils(){}

GLuint ShaderUtils::CreateProgram(const std::vector<GLuint> &shaderList,
								  const std::vector<VertexAttributeBindingData> &attributes,
								  const std::vector<VertexAttributeBindingData> &fragData){
	GLuint program = glCreateProgram();
	bool ok = true;
	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);
	for(size_t iLoop = 0; iLoop < attributes.size(); iLoop++)
		glBindAttribLocation(program, attributes[iLoop].index, attributes[iLoop].strShaderFile );
	for(size_t iLoop = 0; iLoop < fragData.size(); iLoop++)
		glBindFragDataLocation(program, fragData[iLoop].index, fragData[iLoop].strShaderFile );

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		std::cerr << "Linker failure: " << std::endl << strInfoLog << std::endl;
		delete[] strInfoLog;
		ok = false;
	}

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);
	return (ok)?program:ShaderUtils::FAIL_CREATING_PROGRAM;
}

GLuint ShaderUtils::CreateShader(ShaderLoadingData data)
{
	if(!glCreateShader){
		std::cerr << "OpenGL function 'glCreateShader' not found." << std::endl;
		return FAIL_CREATING_SHADER;
	}
	std::string filedata;
	for(std::vector<std::string>::size_type i = 0 ; i < data.strShaderFiles.size();i++){
		QFile qfile(QString::fromStdString(SHADER_RESOURCES+data.strShaderFiles[i]));
		if(!qfile.open(QIODevice::ReadOnly)){
			std::cerr << "Shader file not found: " <<
						 (SHADER_RESOURCES+data.strShaderFiles[i]) << std::endl;
			return FAIL_CREATING_SHADER;
		}
		QTextStream in(&qfile);
		QString line = in.readAll();
		filedata = filedata+line.toStdString();
		qfile.close();
	}
	//std::cout << "Shader file name: " << data.strShaderFiles[0] << std::endl;
	//std::cout << "Shader file length: " << filedata << std::endl;
	//std::cout << "Shader file content: " << filedata << std::endl;

	GLuint shader = glCreateShader(data.eShaderType);

	//load shader file

	const char *strFileData = filedata.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	GLint infoLogLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 1){
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(data.eShaderType)
		{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
//			case GL_COMPUTE_SHADER: strShaderType = "compute"; break;
		}
		if (status == GL_FALSE){
			std::cerr << "Compile failure in " << strShaderType << " shader: "<< std::endl;
			std::cerr << filedata << std::endl;
			std::cerr << "ERROR: "<< std::endl << strInfoLog << std::endl;
		}
		else{
			std::cout << "Compile Status for " << strShaderType << " shader: " << std::endl;
			std::cout << strInfoLog << std::endl;
		}
		delete[] strInfoLog;
	}
	if (status == GL_FALSE)
		shader = FAIL_CREATING_SHADER;
	//	delete[] buffer;
	return shader;
}
GLuint ShaderUtils::CreateProgram(const std::vector<ShaderLoadingData> &shaderList){
	std::vector<VertexAttributeBindingData> dummy;
	return ShaderUtils::CreateProgram(shaderList, dummy);
}
GLuint ShaderUtils::CreateProgram(const std::vector<ShaderLoadingData> &shaderList,
								  const std::vector<VertexAttributeBindingData> &attributes){
	return CreateProgram(shaderList,attributes,std::vector<VertexAttributeBindingData>());
}
GLuint ShaderUtils::CreateProgram(const std::vector<ShaderLoadingData> &shaderList,
								  const std::vector<VertexAttributeBindingData> &attributes,
								  const std::vector<VertexAttributeBindingData> &fragAt){
	std::vector<GLuint> compiledShaderList;
	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++){
		GLuint shaderId = ShaderUtils::CreateShader(shaderList[iLoop]);
		if(shaderId == ShaderUtils::FAIL_CREATING_SHADER){
			std::for_each(compiledShaderList.begin(), compiledShaderList.end(), glDeleteShader);
			return ShaderUtils::FAIL_CREATING_PROGRAM;
		}
		compiledShaderList.push_back(shaderId);
	}
	GLuint program = ShaderUtils::CreateProgram(compiledShaderList,attributes,fragAt);
	std::for_each(compiledShaderList.begin(), compiledShaderList.end(), glDeleteShader);
	if(program==ShaderUtils::FAIL_CREATING_PROGRAM){
		std::cerr << "Failed to Link: "<<std::endl;
		for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++){
			std::vector<std::string> shaderFiles = shaderList[iLoop].strShaderFiles;
			for(std::vector<std::string>::size_type j = 0;j<shaderFiles.size();j++)
				std::cerr << shaderFiles[j] <<std::endl;
		}
		std::cerr <<std::endl;
	}
	return program;
}
bool ShaderUtils::setUniform(GLuint program,const char* uniformName, glm::vec4 val){
	GLint loc = glGetUniformLocation(program,uniformName);
	if(loc >= 0){
		glUniform4fv(loc, 1, glm::value_ptr(val));
		return true;
	}
	return false;
}
bool ShaderUtils::setUniform(GLuint program,const char* uniformName, glm::vec3 val){
	GLint loc = glGetUniformLocation(program,uniformName);
	if(loc >= 0){
		glUniform3fv(loc, 1, glm::value_ptr(val));
		return true;
	}
	return false;
}
bool ShaderUtils::setUniform(GLuint program,const char* uniformName, glm::vec2 val){
	GLint loc = glGetUniformLocation(program,uniformName);
	if(loc >= 0){
		glUniform2fv(loc, 1, glm::value_ptr(val));
		return true;
	}
	return false;
}
bool ShaderUtils::setUniform(GLuint program,const char* uniformName, glm::mat4 val){
	GLint loc = glGetUniformLocation(program,uniformName);
	if(loc >= 0){
		glUniformMatrix4fv(loc, 1, GL_FALSE,glm::value_ptr(val));
		return true;
	}
	return false;
}
bool ShaderUtils::setUniform(GLuint program,const char* uniformName, glm::mat3 val){
	GLint loc = glGetUniformLocation(program,uniformName);
	if(loc >= 0){
		glUniformMatrix3fv(loc, 1, GL_FALSE,glm::value_ptr(val));
		return true;
	}
	return false;
}
bool ShaderUtils::setUniform(GLuint program,const char* uniformName, float val){
	GLint loc = glGetUniformLocation(program,uniformName);
	if(loc >= 0){
		glUniform1f(loc, val);
		return true;
	}
	return false;
}
bool ShaderUtils::setUniform(GLuint program,const char* uniformName, int val){
	GLint loc = glGetUniformLocation(program,uniformName);
	if(loc >= 0){
		glUniform1i(loc, val);
		return true;
	}
	return false;
}

bool ShaderUtils::setUniformTexture(GLuint program,const char* uniformName, GLuint val){
	return setUniform(program,uniformName,(int)val);
}


bool ShaderUtils::setUniform(GLuint program,const char* uniformName, bool val){
	int valInt = (val)?1:0;
	return ShaderUtils::setUniform(program,uniformName,valInt);
}

void ShaderUtils::deleteBuffer(GLuint* b){
	glDeleteBuffers(1,b);
}
