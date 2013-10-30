#include "normalrenderer.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Common/Constants.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_NORMAL 1
#define VERTEX_FLAGS 2
NormalRenderer::NormalRenderer():
	Renderer("Normal Vectors")
{
	config = (NormalRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+1.0f;
}

NormalRenderer::~NormalRenderer(){
	if(config)
		delete config;
}
void NormalRenderer::glewIsReadyRenderer(){
	config = new NormalRendererConfig();
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/NormalRenderer/nr.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("Rendering/Renderers/NormalRenderer/nr.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/NormalRenderer/nr.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData selectAttr = {VERTEX_NORMAL, "VertexNormal"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData flagAttr = {VERTEX_FLAGS, "VertexFlags"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(selectAttr);
	attributeList.push_back(flagAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	sizeRatio = 1.0f;
}

void NormalRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	//Matrix
	glUseProgram(theProgram);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(theProgram, "NormalLength", sizeRatio*config->normalLenght);
	ShaderUtils::setUniform(theProgram, "NormalHeadColor", config->normalHeadColor);
	ShaderUtils::setUniform(theProgram, "NormalTailColor", config->normalTailColor);
	ShaderUtils::setUniform(theProgram, "WireFrameColor", config->wireframeColor);
	ShaderUtils::setUniform(theProgram, "WireFrameOption", config->wireFrameOption);

	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_NORMAL); // Vertex selection
	glEnableVertexAttribArray(VERTEX_FLAGS); // Vertex selection
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexNormalDataBufferObject);
	glVertexAttribPointer( VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_NORMAL); // Vertex selection
	glDisableVertexAttribArray(VERTEX_FLAGS); // Vertex selection

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

}
QWidget* NormalRenderer::getRendererConfigWidget(){
	return config;
}

bool NormalRenderer::hasRendererConfigWidget(){
	return true;
}

void NormalRenderer::applyConfigChanges(RModel *){
	config->readConfig();
}
bool NormalRenderer::rmodelChanged(RModel* rmodel){
	if(rmodel->bounds.size()<6 || !rmodel->getOriginalModel() ||
			rmodel->getModelType()==vis::CONSTANTS::VERTEX_CLOUD ||
			rmodel->getModelType()==vis::CONSTANTS::NO_MODEL)
		return false;
	glm::vec3 minVector = glm::vec3(rmodel->bounds[0],rmodel->bounds[1],rmodel->bounds[2]);
	glm::vec3 maxVector = glm::vec3(rmodel->bounds[3],rmodel->bounds[4],rmodel->bounds[5]);
	float distance = glm::distance(minVector,maxVector);
	sizeRatio = distance / 50.0f;
	return true;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(NormalRenderer);
