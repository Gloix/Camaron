#include "Rendering/Renderers/SceneHelpers.h"

#include <glm/glm.hpp>
#include <math.h>
#include <GL/glew.h>
#include "Rendering/RModel/rmodel.h"
#include "Rendering/SimpleShaders/SimpleShader.h"

#include <iostream>
//singleton class. Provides visual helpers inside the rendered scene.
//Since it is a singleton class, it only supports single OpenGL context Applications.

SceneHelpers::SceneHelpers(){
	//Initialize shaders
	std::cout << "Initializing Scene Helpers Shaders\n";
	simpleShader = new SimpleShader();
	std::cout << "*** Scene Helpers Shaders Loaded\n";
	showAxes = StartingShowAxesValue;
	lastModel = NULL;
}

SceneHelpers::~SceneHelpers(){
	delete simpleShader;
	lastModel = NULL;
}
SceneHelpers* SceneHelpers::theInstance = NULL;
bool SceneHelpers::StartingShowAxesValue=true;

void SceneHelpers::setAxesVisible(bool value){
	showAxes = value;
	StartingShowAxesValue=value;
}

void SceneHelpers::Initialize(){
	std::cout << "Initializing Scene Helpers\n";
	SceneHelpers* ins = new SceneHelpers();
	theInstance = ins;
	std::cout << "** Scene Helpers Initialized.\n";
}

void SceneHelpers::InitializeByRModel(RModel* model){

	if (!model) {
		std::cerr << "Null rmodel in SceneHelpers::InitializedByRModel.\n";
		return;
	}
	lastModel=model;
	//initialize helpers geometry
	if (model->bounds.size()==6){

		float xmin = model->bounds[0];
		float ymin = model->bounds[1];
		float zmin = model->bounds[2];
		float xmax = model->bounds[3];
		float ymax = model->bounds[4];
		float zmax = model->bounds[5];

		float maxlength = std::max(std::max(xmax-xmin, ymax-ymin),zmax-zmin);
		float length = maxlength*0.25f;
		float thickness = length*0.035f;

		float verts[]={ xmin, ymin, zmin, //X in XY
						xmin+length, ymin, zmin,
						xmin+thickness, ymin+thickness, zmin,
						xmin, ymin, zmin, //Y in XY
						xmin+thickness, ymin+thickness, zmin,
						xmin, ymin+length, zmin,
						xmin, ymin, zmin, //X in ZX
						xmin+length, ymin, zmin,
						xmin+thickness, ymin, zmin+thickness,
						xmin, ymin, zmin, //Z in ZX
						xmin, ymin, zmin+length,
						xmin+thickness, ymin, zmin+thickness,
						xmin, ymin, zmin, //Y in YZ
						xmin, ymin+length, zmin,
						xmin, ymin+thickness, zmin+thickness,
						xmin, ymin, zmin, //Z in YZ
						xmin, ymin, zmin+length,
						xmin, ymin+thickness, zmin+thickness}; //the length must match "float_count"

		float z = 0.0f;//zero
		float b = 1.0f;//brighter
		float d = 0.8f;//darker
		float cols[]={  b, z, z,
						b, z, z,
						b, z, z,
						z, b, z,
						z, b, z,
						z, b, z,
						d, z, z,
						d, z, z,
						d, z, z,
						z, z, b,
						z, z, b,
						z, z, b,
						z, d, z,
						z, d, z,
						z, d, z,
						z, z, d,
						z, z, d,
						z, z, d};

		//if (vao[0]>=) glDeleteVertexArrays(vao,1);

		GLuint vbo[2];
		glGenVertexArrays((GLuint)1, vao);
		glBindVertexArray(vao[0]);
		glGenBuffers((GLuint)2, vbo);
		//position buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, float_count*sizeof(float), verts,GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);
		//color buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, float_count*sizeof(float), cols,GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);//reset bind
	}
}


//Clean up and force the instance to be re-generated on next getInstance call
void SceneHelpers::reset(){
	if (theInstance) {
		delete theInstance;
		theInstance = NULL;
	}
}

SceneHelpers* SceneHelpers::getInstance(){
	if (!theInstance) Initialize();
	return theInstance;
}

void SceneHelpers::drawHelpers(RModel* model){
	//we don't check if model is NULL at the begining because some helpers may be independent from it.
	//Note that helpers will be drawn after the rest of the scene. Must not clear the color buffer!
	if (model){ //model dependant helpers
		if (model!=lastModel){
			InitializeByRModel(model);
		}
		if (this->showAxes==true){
			glClear(GL_DEPTH_BUFFER_BIT);
			glm::mat4 modelmatrix=glm::mat4(1.0f);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			simpleShader->setModelMatrix(modelmatrix);
			simpleShader->setViewMatrix(model->getMVP());//load all in view
			simpleShader->setProjectionMatrix(modelmatrix);
			simpleShader->loadShader();
			glBindVertexArray(vao[0]);
			glDrawArrays(GL_TRIANGLES, 0, vertex_count);
			glBindVertexArray(0);
			simpleShader->unloadShader();
		}
	}
}


