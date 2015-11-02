#include <algorithm>
#include "isolinerenderer.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Common/Constants.h"
#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS 1
#define VERTEX_SCALARPROP 2
IsolineRenderer::IsolineRenderer():
    Renderer("Isolines")
{
    config = (IsolineRendererConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+1.0f;
}

IsolineRenderer::~IsolineRenderer(){
	if(config)
		delete config;
}
void IsolineRenderer::glewIsReadyRenderer(){
    config = new IsolineRendererConfig();
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
    vertexShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
    geometryShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
    fragmentShaderData.addFile("Rendering/Renderers/IsolineRenderer/ir.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


    //VertexAttributeBindingData selectAttr = {VERTEX_NORMAL, "VertexNormal"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData flagAttr = {VERTEX_FLAGS, "VertexFlags"};
    VertexAttributeBindingData scalarAttr = {VERTEX_SCALARPROP, "VertexScalar"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
    attributeList.push_back(scalarAttr);
	attributeList.push_back(flagAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	sizeRatio = 1.0f;
}

void IsolineRenderer::draw(RModel* rmodel){
    config->setModel(rmodel);
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	//Matrix
	glUseProgram(theProgram);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
    ShaderUtils::setUniform(theProgram, "IsolinesSteps", config->isolinesSteps);
    ShaderUtils::setUniform(theProgram, "IsolinesStepsN", (int)config->isolinesSteps.size());
    ShaderUtils::setUniform(theProgram, "GradientStartColor", config->gradientStartColor);
    ShaderUtils::setUniform(theProgram, "GradientEndColor", config->gradientEndColor);
	ShaderUtils::setUniform(theProgram, "WireFrameColor", config->wireframeColor);
	ShaderUtils::setUniform(theProgram, "WireFrameOption", config->wireFrameOption);

    std::vector<VScalarDef*> scalarDefs = rmodel->scalarDefs;
    std::vector<VScalarDef*>::iterator pos = std::find(scalarDefs.begin(), scalarDefs.end(), config->selectedScalarDef);
	int selectedScalarDefIndex;
    if(pos != scalarDefs.end()) {
		selectedScalarDefIndex = (int)(pos - scalarDefs.begin());
    } else {
		return;
    }


	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
    glEnableVertexAttribArray(VERTEX_SCALARPROP); // Vertex scalars
    glEnableVertexAttribArray(VERTEX_FLAGS); // Vertex flags
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
    // Map index 1 to the scalar buffer
    glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexScalarDataBufferObject);
	glVertexAttribPointer( VERTEX_SCALARPROP, 1, GL_FLOAT, GL_FALSE, scalarDefs.size()*sizeof(float),
						   (GLubyte *)selectedScalarDefIndex );
    // Map index 2 to the flags buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
    glDisableVertexAttribArray(VERTEX_SCALARPROP); // Vertex selection
	glDisableVertexAttribArray(VERTEX_FLAGS); // Vertex selection

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

}
BaseRendererConfig* IsolineRenderer::getRendererConfigWidget(){
	return config;
}

bool IsolineRenderer::hasRendererConfigWidget(){
	return true;
}

void IsolineRenderer::applyConfigChanges(RModel *){
	config->readConfig();
}
bool IsolineRenderer::rmodelChanged(RModel* rmodel){
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
REGISTER_RENDERER(IsolineRenderer);
