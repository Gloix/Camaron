#include "cartesiancoordinatesaxis.h"
#include "Rendering/RModel/rmodel.h"
#include "Utils/shaderutils.h"
#include "Utils/matrixtransformationutils.h"
#include <glm/gtc/type_ptr.hpp>

#define INSERT_NEW_VERTEX_BOUND_BOX(x,y,z) \
	coordinatesBox.push_back(glm::vec3(bounds[x],bounds[y],bounds[z]))

#define POSITION_ATTRIBUTE 0
CartesianCoordinatesAxis::CartesianCoordinatesAxis():Renderer("Cartesian Axis"){
	config = (CartesianCoordinatesAxisConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+500.0f;
	coordinatesBoxPointer = 0;
	theProgram = 0;
	pointsPointer = 0;
}

CartesianCoordinatesAxis::~CartesianCoordinatesAxis(){
	if(config)
		delete config;
	if(coordinatesBoxPointer)
		ShaderUtils::deleteBuffer(&coordinatesBoxPointer);
	if(pointsPointer)
		ShaderUtils::deleteBuffer(&pointsPointer);
}

bool CartesianCoordinatesAxis::rmodelChanged(RModel* rmodel){
	if(!rmodel || !rmodel->getOriginalModel() || rmodel->bounds.size()!=6)
		return false;
	std::vector<float>& bounds = rmodel->bounds;
	int XMIN = 0;
	int XMAX = 3;
	int YMIN = 1;
	int YMAX = 4;
	int ZMIN = 2;
	int ZMAX = 5;
	points.clear();
	points.push_back(glm::vec3(0.0f,0.0f,0.0f));
	points.push_back(glm::vec3(bounds[XMIN],bounds[YMIN],bounds[ZMIN]));
	points.push_back(glm::vec3(bounds[XMAX],bounds[YMAX],bounds[ZMAX]));
	coordinatesBox.clear();
	//FACE XY, z min
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMIN,ZMIN);
	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMIN,ZMIN);

	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMIN,ZMIN);
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMAX,ZMIN);

	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMIN,ZMIN);//not part of XY, zmin face
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMIN,ZMAX);//not part of XY, zmin face

	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMAX,ZMIN);
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMAX,ZMIN);

	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMAX,ZMIN);
	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMIN,ZMIN);

	//FACE XY, z MAX
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMIN,ZMAX);
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMAX,ZMAX);

	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMIN,ZMAX);
	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMIN,ZMAX);

	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMAX,ZMAX);
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMAX,ZMAX);

	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMAX,ZMAX);
	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMIN,ZMAX);

	//as

	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMIN,ZMIN);
	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMIN,ZMAX);
	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMAX,ZMIN);
	INSERT_NEW_VERTEX_BOUND_BOX(XMAX,YMAX,ZMAX);
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMAX,ZMIN);
	INSERT_NEW_VERTEX_BOUND_BOX(XMIN,YMAX,ZMAX);
	if(coordinatesBoxPointer)
		ShaderUtils::setDataBuffer<glm::vec3>(coordinatesBoxPointer,coordinatesBox);
	else
		coordinatesBoxPointer = ShaderUtils::createDataBuffer<glm::vec3>(coordinatesBox);
	if(pointsPointer)
		ShaderUtils::setDataBuffer<glm::vec3>(pointsPointer,points);
	else
		pointsPointer = ShaderUtils::createDataBuffer<glm::vec3>(points);
	return true;
}
void CartesianCoordinatesAxis::glewIsReadyRenderer(){
	config = new CartesianCoordinatesAxisConfig();
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/CartesianCoordinatesAxis/cca.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/CartesianCoordinatesAxis/cca.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);

	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	applyConfigChanges();
}

void CartesianCoordinatesAxis::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	glUseProgram(theProgram);

	GLfloat originalPointSize;
	glGetFloatv(GL_POINT_SIZE,&originalPointSize);
	GLfloat originalLineWidth;
	glGetFloatv(GL_LINE_WIDTH,&originalLineWidth);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	if(config->drawBoundingBox){
		glLineWidth(config->boundingBoxLineWidth);
		ShaderUtils::setUniform(theProgram, "BaseColor",config->boundingBoxColor);
		glBindBuffer(GL_ARRAY_BUFFER, coordinatesBoxPointer);
		int ignoreAxis = 0;
		if(config->drawAxix)
			ignoreAxis = 6;
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
							   (GLubyte *)NULL );
		glDrawArrays(GL_LINES, ignoreAxis, coordinatesBox.size()-ignoreAxis ); // 0-5 axis
	}
	if(config->drawAxix){
		glLineWidth(config->axisLineWidth);
		//X inc AXIS
		ShaderUtils::setUniform(theProgram, "BaseColor",config->xAxisColor);
		glBindBuffer(GL_ARRAY_BUFFER, coordinatesBoxPointer);
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
							   (GLubyte *)NULL );
		glDrawArrays(GL_LINES, 0, 2 ); // 3 axis
		//Y inc AXIS
		ShaderUtils::setUniform(theProgram, "BaseColor",config->yAxisColor);
		glBindBuffer(GL_ARRAY_BUFFER, coordinatesBoxPointer);
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
							   (GLubyte *)NULL );
		glDrawArrays(GL_LINES, 2, 2 ); // 3 axis
		//Z inc AXIS
		ShaderUtils::setUniform(theProgram, "BaseColor",config->zAxisColor);
		glBindBuffer(GL_ARRAY_BUFFER, coordinatesBoxPointer);
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
							   (GLubyte *)NULL );
		glDrawArrays(GL_LINES, 4, 2 ); // 3 axis
	}
	if(config->drawExtremePoints){
		glPointSize(config->extremePointSize);
		ShaderUtils::setUniform(theProgram, "BaseColor",config->extremePointsColor);
		//DRAW POINTS
		glBindBuffer(GL_ARRAY_BUFFER, pointsPointer);
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
							   (GLubyte *)NULL );
		glDrawArrays(GL_POINTS, 1, points.size()-1); // 3 axis
	}
	if(config->drawOriginPoint){
		glPointSize(config->originPointSize);
		ShaderUtils::setUniform(theProgram, "BaseColor",config->originPointColor);
		//DRAW POINTS
		glBindBuffer(GL_ARRAY_BUFFER, pointsPointer);
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
							   (GLubyte *)NULL );
		glDrawArrays(GL_POINTS, 0, 1); // 3 axis
	}
	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glPointSize(originalPointSize);
	glLineWidth(originalLineWidth);
	glUseProgram(0);
}

void CartesianCoordinatesAxis::applyConfigChanges(RModel*){
	config->readConfig();
}

BaseRendererConfig* CartesianCoordinatesAxis::getRendererConfigWidget(){
	return config;
}

bool CartesianCoordinatesAxis::hasRendererConfigWidget(){
	return true;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(CartesianCoordinatesAxis);
