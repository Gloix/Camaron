#include "Rendering/Renderer.h"
#include "Rendering/RModel/rmodel.h"
#include "Utils/matrixtransformationutils.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
Renderer::Renderer(std::string newname):name(newname){
	this->ok = true;
	rendererWeight = RENDERER_WEIGHT_BASE;
}

Renderer::~Renderer(){}
void Renderer::draw(RModel*){}
void Renderer::rendererSelected(RModel*){}
bool Renderer::rmodelChanged(RModel* rmodel){
	return rmodel->getOriginalModel()&&
			rmodel->getOriginalModel()->getModelType()!=vis::CONSTANTS::VERTEX_CLOUD&&
			rmodel->getOriginalModel()->getModelType()!=vis::CONSTANTS::NO_MODEL;
}
QWidget*  Renderer::getRendererConfigWidget(){
	return (QWidget*)0;
}

bool  Renderer::hasRendererConfigWidget(){
	return false;
}
const char* Renderer::getRendererName( ) const{
	return this->name.c_str();
}

bool Renderer::isOk(){
	return this->ok;
}
void Renderer::applyConfigChanges(RModel *){}

glm::mat4 Renderer::getMV(RModel* rmodel){
	return rmodel->getMV();
}
glm::mat4 Renderer::getMVP(RModel* rmodel){
	return rmodel->getMVP();
}
glm::mat4 Renderer::getMVPVP(RModel* rmodel){
	return rmodel->getMVPVP();
}
glm::vec3 Renderer::isDisplayedViewPortCheck(vis::Element* e, RModel* rmodel){
	//if check viewport con coords viewport
	glm::vec3 wpos;
	if(rmodel->isOrthoProjectionInUse())
		wpos = glm::vec3(rmodel->getMVPVP()*glm::vec4(e->getGeometricCenter(),1.0f));
	else{
		glm::vec4 mvpCoord = rmodel->getMVP()*glm::vec4(e->getGeometricCenter(),1.0f);
		mvpCoord /= mvpCoord.w;
		wpos = glm::vec3(rmodel->getViewPortMatrix()*mvpCoord);
	}
	glm::vec2 vportSize = rmodel->getViewPortSize();
	wpos.y = vportSize.y-wpos.y;
	if(wpos.x<0 ||
			wpos.y<0 ||
			wpos.x>vportSize.x ||
			wpos.y>vportSize.y)
		return glm::vec3(-1.0f,-1.0f,-1.0f);
	return wpos;
}

bool Renderer::isDisplayed(vis::Element*, RModel*){
	return true;
}
void Renderer::glewIsReadyRenderer(){}

float Renderer::getRendererWeight(){
	return rendererWeight;
}

void Renderer::setRendererWeight(float f){
	rendererWeight = f;
}
bool Renderer::isValidAsSecondaryRenderer(){
	return true;
}
