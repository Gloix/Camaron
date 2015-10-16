#include "phongperpixel.h"
#include "Rendering/Renderers/ShadingEffectsRender/shadingeffectsrenderconfig.h"
#include "Utils/shaderutils.h"
#include "Utils/matrixtransformationutils.h"
#include "Rendering/RModel/rmodel.h"

#define POSITION_ATTRIBUTE 0
#define NORMAL_ATTRIBUTE 1
#define FLAG_ATTRIBUTE 2
PhongPerPixel::PhongPerPixel():
	Renderer("Phong Shading per Pixel")
{
	config = (ShadingEffectsRenderConfig*)0;
	rendererWeight = RENDERER_WEIGHT_BASE+0.0002f;
}
PhongPerPixel::~PhongPerPixel(){
	if(config)
		delete config;
}
void PhongPerPixel::glewIsReadyRenderer(){
	config = new ShadingEffectsRenderConfig();
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("Rendering/Renderers/PhongPerPixel/ppp.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("Rendering/Renderers/PhongPerPixel/ppp.frag");
	fragmentShaderData.addFile("Rendering/Renderers/ShaderCommonFunctions/phong.su");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData normalAttr = {NORMAL_ATTRIBUTE, "VertexNormal"};
	VertexAttributeBindingData flagAttr = {FLAG_ATTRIBUTE, "VertexFlag"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(normalAttr);
	attributeList.push_back(flagAttr);

	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		this->ok = false;
	this->applyConfigChanges();
}

void PhongPerPixel::draw(RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	glm::mat3 normalMatrix = glm::mat3(rmodel->getMV());
	//normalMatrix = glm::inverse(glm::transpose(normalMatrix));
	glUseProgram(theProgram);
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(theProgram, "ModelView",rmodel->getMV());
	ShaderUtils::setUniform(theProgram, "NormalMatrix",normalMatrix);
	ShaderUtils::setUniform(theProgram, "Material.Ka",config->phongConfig.material.Ka);
	ShaderUtils::setUniform(theProgram, "Material.Kd",config->phongConfig.material.Kd);
	ShaderUtils::setUniform(theProgram, "Material.Ks",config->phongConfig.material.Ks);
	ShaderUtils::setUniform(theProgram, "Material.Shininess",config->phongConfig.material.Shininess);
	ShaderUtils::setUniform(theProgram, "TranslucentMode",config->translucentMode);
	ShaderUtils::setUniform(theProgram, "InverseBackColor",config->inverseBackColor);

	ShaderUtils::setUniform(theProgram, "Light.La",config->phongConfig.light.La);
	ShaderUtils::setUniform(theProgram, "Light.Ld",config->phongConfig.light.Ld);
	ShaderUtils::setUniform(theProgram, "Light.Ls",config->phongConfig.light.Ls);
	ShaderUtils::setUniform(theProgram, "Light.Position",config->phongConfig.light.Position);
	ShaderUtils::setUniform(theProgram, "ElementDrawOption",config->elementDrawnOptionConfig.elementDrawnOption);
	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(NORMAL_ATTRIBUTE);
	glEnableVertexAttribArray(FLAG_ATTRIBUTE);
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexNormalDataBufferObject);
	glVertexAttribPointer( NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( FLAG_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );

	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(NORMAL_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(FLAG_ATTRIBUTE); // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);
}

BaseRendererConfig* PhongPerPixel::getRendererConfigWidget(){
	return config;
}

bool PhongPerPixel::hasRendererConfigWidget(){
	return true;
}

void PhongPerPixel::applyConfigChanges(RModel*){
	config->readConfig();
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(PhongPerPixel);
