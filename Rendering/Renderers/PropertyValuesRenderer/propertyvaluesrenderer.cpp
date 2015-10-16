#include "propertyvaluesrenderer.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/Model.h"
#include "Utils/matrixtransformationutils.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_PROPERTY_VALUE_ATTRIBUTE 2
#define VERTEX_FLAGS_ATTRIBUTE 1
PropertyValuesRenderer::PropertyValuesRenderer():Renderer("Properties Renderer")
{
	config = (PropertyValuesRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+2.0f;
}
PropertyValuesRenderer::~PropertyValuesRenderer()
{
	if(config)
		delete config;
}
void PropertyValuesRenderer::glewIsReadyRenderer(){
	config = new PropertyValuesRendererConfig();
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/PropertyValuesRenderer/pvr.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/PropertyValuesRenderer/pvr.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/PropertyValuesRenderer/pvr.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData selectAttr = {VERTEX_PROPERTY_VALUE_ATTRIBUTE, "VertexValue"};
	VertexAttributeBindingData flagAttr = {VERTEX_FLAGS_ATTRIBUTE, "VertexFlags"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(flagAttr);
	attributeList.push_back(selectAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	vertexPropertyValueBuffer = RModel::NULL_BUFFER;
	this->applyConfigChanges();
}
BaseRendererConfig* PropertyValuesRenderer::getRendererConfigWidget(){
	return config;
}

bool PropertyValuesRenderer::hasRendererConfigWidget(){
	return true;
}
void PropertyValuesRenderer::applyConfigChanges(RModel* rmodel){
	config->readConfig();
	if(config->changedEvaluationStrategy || config->evaluationStrategy->getCalculatedValuesCount()==0){
		if(!rmodel||!rmodel->getOriginalModel())
			return;//fail
		if(vertexPropertyValueBuffer!=RModel::NULL_BUFFER)
			glDeleteBuffers(1,&this->vertexPropertyValueBuffer);
		createVertexPropertyData(rmodel);
		vertexPropertyValueBuffer = ShaderUtils::createDataBuffer<GLfloat>(propertyData);
	}
}
void PropertyValuesRenderer::createVertexPropertyData(RModel* rmodel){
	Model* model = rmodel->getOriginalModel();
	if(propertyData.size()!=rmodel->vertexFlagsAttribute.size()){
		if(propertyData.size()<rmodel->vertexFlagsAttribute.size())
			propertyData.resize(rmodel->vertexFlagsAttribute.size());
	}
	model->evaluateUsingEvaluationStrategy(config->evaluationStrategy,propertyData);

}
bool PropertyValuesRenderer::rmodelChanged(RModel* rmodel){
	config->readConfig();
	if(!rmodel || !rmodel->getOriginalModel() ||
			rmodel->getOriginalModel()->isLightWeight()){
		//clear
		if(vertexPropertyValueBuffer!=RModel::NULL_BUFFER)
			glDeleteBuffers(1,&this->vertexPropertyValueBuffer);
		this->vertexPropertyValueBuffer = RModel::NULL_BUFFER;
		std::vector<GLfloat>().swap(propertyData);
		return false;
	}
	if(!rmodel||!rmodel->getOriginalModel()||!config->evaluationStrategy)
		return false;//fail
	if(!rmodel->getOriginalModel() ||
			rmodel->getOriginalModel()->getModelType()==vis::CONSTANTS::VERTEX_CLOUD)
		return false;
	if(vertexPropertyValueBuffer!=RModel::NULL_BUFFER)
		glDeleteBuffers(1,&this->vertexPropertyValueBuffer);
	propertyData.clear();
	propertyData.resize(rmodel->vertexFlagsAttribute.size());
	for(std::vector<float>::size_type i = 0; i< propertyData.size();i++)
		propertyData[i] = config->evaluationStrategy->getNullValue();
	vertexPropertyValueBuffer = ShaderUtils::createDataBuffer<GLfloat>(propertyData);
	return true;
}

void PropertyValuesRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	glUseProgram(theProgram);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(theProgram, "NoValueColor",config->noValueColor);
	ShaderUtils::setUniform(theProgram, "MinValueColor",config->minValueColor);
	ShaderUtils::setUniform(theProgram, "MaxValueColor",config->maxValueColor);
	ShaderUtils::setUniform(theProgram, "minValue",config->evaluationStrategy->getMinValue());
	ShaderUtils::setUniform(theProgram, "maxValue",config->evaluationStrategy->getMaxValue());
	ShaderUtils::setUniform(theProgram, "noValue",config->evaluationStrategy->getNullValue());
	ShaderUtils::setUniform(theProgram, "DrawOnlySelectedElements",config->drawOnlySelectedElements);
	//Wireframe uniforms
	ShaderUtils::setUniform(theProgram, "ViewportMatrix",rmodel->getViewPortMatrix());
	ShaderUtils::setUniform(theProgram, "Line.Width",this->config->wireFrameLineWidth);
	ShaderUtils::setUniform(theProgram, "Line.SelectedWidth",this->config->wireFrameSelectedLineWidth);
	ShaderUtils::setUniform(theProgram, "Line.Color",this->config->wireFrameColors);
	ShaderUtils::setUniform(theProgram, "Line.SelectedColor",this->config->wireFrameSelectedColors);
	ShaderUtils::setUniform(theProgram, "Line.isDrawn",this->config->drawWireFrame);
	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_PROPERTY_VALUE_ATTRIBUTE); // Vertex selection
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexPropertyValueBuffer);
	glVertexAttribPointer( VERTEX_PROPERTY_VALUE_ATTRIBUTE, 1, GL_FLOAT, GL_FALSE, 0,
							(GLubyte *)NULL );

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );

	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_PROPERTY_VALUE_ATTRIBUTE); // Vertex selection
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(PropertyValuesRenderer);
