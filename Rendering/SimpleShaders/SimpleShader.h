#ifndef _SIMPLESHADER_H_
#define _SIMPLESHADER_H_

#include <string>
#include <stdarg.h>
#include <algorithm>
#include <GL/glew.h>
#include <glm/ext.hpp>
class SimpleShader{

private:
	GLuint program;
	GLuint u_projectionMatrix_location;
	GLuint u_modelMatrix_location;
	GLuint u_viewMatrix_location;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
public:

	SimpleShader(){
		if(!glCreateShader){
			std::cerr << "SimpleShader Ctor.: OpenGL function 'glCreateShader' not found." << std::endl;
			return;
		}

		program = glCreateProgram();

		//Vertex
		//std::string vertex=" #version 400 \n in  vec3 in_Position;\n in  vec3 in_Color;\n out vec3 ex_Color;\nuniform mat4 u_ModelMatrix;\nuniform mat4 u_ViewMatrix;\nuniform mat4 u_ProjectionMatrix;\nvoid main(void){\ngl_Position = vec4(in_Position, 1.0);\nex_Color = in_Color;\n}";
		std::string vertex="#version 400\n\
				in  vec3 in_Position;\n\
				in  vec3 in_Color;\n\
				out vec3 ex_Color;\n\
				\n\
				uniform mat4 u_ModelMatrix;\n\
				uniform mat4 u_ViewMatrix;\n\
				uniform mat4 u_ProjectionMatrix;\n\
				\n\
				void main(void){\n\
					gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(in_Position, 1.0);\n\
					ex_Color = in_Color;\n\
				}";
		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		const char *strVertex = vertex.c_str();
		glShaderSource(vshader, 1, &strVertex, NULL);
		glCompileShader(vshader);
		//delete [] strVertex;
		glAttachShader(program, vshader);

		//Fragment
		//std::string fragment="#version 400 \nin  vec3 ex_Color;\nout vec4 out_Color;\nuniform mat4 u_ModelMatrix;\nuniform mat4 u_ViewMatrix;\nuniform mat4 u_ProjectionMatrix;\n\nvoid main(void){\nout_Color = vec4(ex_Color, 1.0);\n}";
		std::string fragment="#version 400\n\
				in  vec3 ex_Color;\n\
				\n\
				out vec4 out_Color;\n\
				\n\
				uniform mat4 u_ModelMatrix;\n\
				uniform mat4 u_ViewMatrix;\n\
				uniform mat4 u_ProjectionMatrix;\n\
				\n\
				void main(void){\n\
					out_Color = vec4(ex_Color, 1.0);\n\
				}";
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
		const char *strFragment = fragment.c_str();
		glShaderSource(fshader, 1, &strFragment, NULL);
		glCompileShader(fshader);
		//delete [] strFragment;
		glAttachShader(program, fshader);

		glBindAttribLocation(program, 0, "in_Position");
		glBindAttribLocation(program, 1, "in_Color");
		glLinkProgram(program);
		glUseProgram(program);

		u_projectionMatrix_location = glGetUniformLocation(program, "u_ProjectionMatrix");
		u_modelMatrix_location = glGetUniformLocation(program, "u_ModelMatrix");
		u_viewMatrix_location = glGetUniformLocation(program, "u_ViewMatrix");

	}

	void setProjectionMatrix(glm::mat4 val){
		projectionMatrix = val;
	}

	void setModelMatrix(glm::mat4 val){
		modelMatrix = val;
	}

	void setViewMatrix(glm::mat4 val){
		viewMatrix = val;
	}

	void loadShader(){
		glUseProgram(program);
		glUniformMatrix4fv(u_projectionMatrix_location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(u_modelMatrix_location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(u_viewMatrix_location, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}

	void unloadShader(){
		glUseProgram(0);
	}

	~SimpleShader(){
		glDeleteProgram(program);
	}
};

#endif
