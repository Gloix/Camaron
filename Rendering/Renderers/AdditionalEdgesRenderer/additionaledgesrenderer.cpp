#include <algorithm>
#include "additionaledgesrenderer.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/rmodel.h"
#include "Common/Constants.h"
#define POSITION_ATTRIBUTE 0
#define COLOR_ATTRIBUTE 1
AdditionalEdgesRenderer::AdditionalEdgesRenderer():
	Renderer("Additional Edges")
{
	rendererWeight = RENDERER_WEIGHT_BASE+1.0f;
}

AdditionalEdgesRenderer::~AdditionalEdgesRenderer(){

}
void AdditionalEdgesRenderer::glewIsReadyRenderer(){
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/AdditionalEdgesRenderer/aer.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/AdditionalEdgesRenderer/aer.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);

	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData colorAttr = {COLOR_ATTRIBUTE, "VertexColor"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(colorAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
}

void AdditionalEdgesRenderer::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	glUseProgram(theProgram);
	//Matrix
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());

	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(COLOR_ATTRIBUTE); // Vertex scalars
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->edgeVertexPositionsDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the scalar buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->edgeColorDataBufferObject);
	glVertexAttribPointer( COLOR_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	glDrawArrays(GL_LINES, 0, rmodel->nAdditionalEdges*2 );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(COLOR_ATTRIBUTE); // Vertex selection

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);

}
BaseRendererConfig* AdditionalEdgesRenderer::getRendererConfigWidget(){
	return NULL;
}

bool AdditionalEdgesRenderer::hasRendererConfigWidget(){
	return false;
}

void AdditionalEdgesRenderer::applyConfigChanges(RModel *){
	//config->readConfig();
}
bool AdditionalEdgesRenderer::rmodelChanged(RModel* rmodel){
	if(rmodel->bounds.size()<6 || !rmodel->getOriginalModel() ||
			rmodel->getModelType()==vis::CONSTANTS::NO_MODEL)
		return false;
	return true;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(AdditionalEdgesRenderer);
