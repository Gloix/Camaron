#include "renderelementsids.h"
#include "ui_renderelementsids.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Element.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "SelectionStrategies/Selection.h"
#include "Rendering/RModel/rmodel.h"
#include "Rendering/Renderer.h"
#include "Utils/qtutils.h"
#include "Utils/shaderutils.h"
#include "Utils/fileutils.h"
#include "Utils/openglutils.h"
#include <QPainter>
#include <string>
#define POSITION_ATTRIBUTE 0
#define VERTEX_FLAGS_ATTRIBUTE 1
#define PP_IDS_ATTRIBUTE 2
#define VERTEX_IDS_ATTRIBUTE 3
#define NORMAL_ATTRIBUTE 4
RenderElementsIds::RenderElementsIds(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint ),
	GenericConfigWidget("Ids"),
	ui(new Ui::RenderElementsIds),
	container()
{
	ui->setupUi(this);
	ui->widget_gpu->hide();
	readConfig();
	container.setCapacity(nElements);
	shaderOk = true;
}

RenderElementsIds::~RenderElementsIds()
{
	delete ui;
	OpenGLUtils::deleteTexture( textureId );
}
#include <iostream>
void RenderElementsIds::renderIds(Renderer* renderer, RModel* rmodel,  Selection* selection,QPainter& painter){
	Model* model = rmodel->getOriginalModel();
	if(!model)
		return;
	readConfig();
	if(useShaderAcceleration&&shaderOk){
		renderIdsUsingShaders(renderer,rmodel);
		return;
	}else if(model->isLightWeight())
		return;//fail, cant render ids using CPU for light weight models - not yet
	painter.setPen( QColor((int)(textColor.x*255),(int)(textColor.y*255),(int)(textColor.z*255),(int)(textColor.w*255)));

	QFont font = painter.font();
	int newPointSize = (int)(font.pointSize()*fontSize);
	font.setPointSize(newPointSize);
	painter.setFont(font);
	viewPortCenter = glm::vec3(rmodel->getViewPortSize()/2.0f,-0.5f);
	if(container.getCapacity()!=nElements)
		container.setCapacity(nElements);
	if(elementIdDrawnOption == DRAW_ONLY_SELECTED){
		if(selection->getSelectionType()<elementsType)
			return;
		std::unordered_map<int,vis::Element*>& selectedElements = selection->getSelectedElements();
		if(elementsType==selection->getSelectionType()){ ///render ids of the same time of the selection
			for(std::unordered_map<int,vis::Element*>::const_iterator it = selectedElements.begin();
				it!=selectedElements.end();it++)
				processElement(it->second,renderer,rmodel);
		}
		else if(elementsType == vis::CONSTANTS::POLYGON){
			std::unordered_map<int,int> processedElements;
			for(std::unordered_map<int,vis::Element*>::const_iterator it = selectedElements.begin();
				it!=selectedElements.end();it++){
				vis::Polyhedron* polyhedron = (vis::Polyhedron*)it->second;
				if(discardSelection(polyhedron))
					continue;
				std::vector<vis::Polygon*>& polygons = polyhedron->getPolyhedronPolygons();
				for( vis::Polygon* polygon : polygons ){
					if(processedElements.count(polygon->getId())==0){
						processElement(polygon,renderer,rmodel,true);
						processedElements[polygon->getId()] = 1;
					}
				}
			}
			processedElements.clear();
		}
		else if(elementsType == vis::CONSTANTS::VERTEX){
			std::unordered_map<int,int> processedElements;
			if(selection->getSelectionType()==vis::CONSTANTS::POLYGON){
				for(std::unordered_map<int,vis::Element*>::const_iterator it = selectedElements.begin();
					it!=selectedElements.end();it++){
					vis::Polygon* polygon = (vis::Polygon*)it->second;
					if(discardSelection(polygon))
						continue;
					std::vector<vis::Vertex*>& vertices = polygon->getVertices();
					for( vis::Vertex* vertex : vertices ){
						if(processedElements.count(vertex->getId())==0){
							processElement(vertex,renderer,rmodel,true);
							processedElements[vertex->getId()] = 1;
						}
					}
				}
			}
			else if(selection->getSelectionType()==vis::CONSTANTS::POLYHEDRON){
				for(std::unordered_map<int,vis::Element*>::const_iterator it = selectedElements.begin();
					it!=selectedElements.end();it++){
					vis::Polyhedron* polyhedron = (vis::Polyhedron*)it->second;
					if(discardSelection(polyhedron))
						continue;
					std::vector<vis::Polygon*>& polygons = polyhedron->getPolyhedronPolygons();
					for( vis::Polygon* polygon : polygons ){
						std::vector<vis::Vertex*>& vertices = polygon->getVertices();
						for( vis::Vertex* vertex : vertices ){
							if(processedElements.count(vertex->getId())==0){
								processElement(vertex,renderer,rmodel,true);
								processedElements[vertex->getId()] = 1;
							}
						}
					}
				}
			}
			processedElements.clear();
		}

		renderOrderedElementsIds(painter);
		container.clear();
		return;
	}
	switch(elementsType){
		case vis::CONSTANTS::VERTEX:{
			renderIds(renderer,rmodel,painter,(VertexCloud*)model);
			break;
		}
		case vis::CONSTANTS::POLYGON:{
			if(model->getModelType()== vis::CONSTANTS::VERTEX_CLOUD)
				return;
			renderIds(renderer,rmodel,painter,(PolygonMesh*)model);
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			if(model->getModelType()!= vis::CONSTANTS::POLYHEDRON_MESH)
				return;
			renderIds(renderer,rmodel,painter,(PolyhedronMesh*)model);
			break;
		}
	}
	container.clear();


}
void RenderElementsIds::renderIds(Renderer* renderer, RModel* rmodel, QPainter& painter,VertexCloud* model){
	std::vector<vis::Vertex*>& vertices = model->getVertices();
	for( vis::Vertex* vertex : vertices )
		processElement(vertex,renderer,rmodel);
	renderOrderedElementsIds(painter);
}

void RenderElementsIds::renderIds(Renderer* renderer, RModel* rmodel, QPainter& painter,PolygonMesh* mesh){
	std::vector<vis::Polygon*>& polygons = mesh->getPolygons();
	for( vis::Polygon* polygon : polygons )
		processElement(polygon,renderer,rmodel);
	renderOrderedElementsIds(painter);
}

void RenderElementsIds::renderIds(Renderer* renderer, RModel* rmodel, QPainter& painter,PolyhedronMesh* mesh){
	std::vector<vis::Polyhedron*>& polyhedrons = mesh->getPolyhedrons();
	glm::vec3 viewPortCenter(rmodel->getViewPortSize()/2.0f,-0.5f);
	for( vis::Polyhedron* polyhedron : polyhedrons )
		processElement(polyhedron,renderer,rmodel);
	renderOrderedElementsIds(painter);
}

void RenderElementsIds::processElement(vis::Element* e,Renderer* renderer, RModel* rmodel,bool ignoreSelectionl){
	if(discard(e,ignoreSelectionl))
		return;
	glm::vec3 viewPortPosition = renderer->isDisplayedViewPortCheck(e,rmodel);
	if(viewPortPosition.x != -1.0f){
		if(!useDepthStrategy){
			viewPortPosition.z *= 2.0f;
			viewPortPosition.z = glm::distance(viewPortPosition,viewPortCenter);
		}
		container.addElement(viewPortPosition,e->getId());
	}
}

void RenderElementsIds::renderOrderedElementsIds(QPainter& painter){
	std::vector<std::pair<glm::vec2,int> >& elementsToDraw = container.getOrderedElements();
	for( std::pair<glm::vec2,int> pair : elementsToDraw ){
		glm::vec2& vec = pair.first;
		const QFont& font = painter.font();
		QString toDraw = QString::number(pair.second);
		int height = QFontMetrics( font ).height();
		int width = QFontMetrics( font ).width(toDraw);
		if(textFillColor.w > 0.0f){
			painter.fillRect(vec.x-width/2,vec.y-height/2,width,height,
							 QColor((int)(textFillColor.x*255),
									(int)(textFillColor.y*255),
									(int)(textFillColor.z*255),
									(int)(textFillColor.w*255)));

		}
		painter.drawText(vec.x-width/2,vec.y+height/2,toDraw);

	}
}

void RenderElementsIds::setElementsType(int n){
	elementsType = n;
}
void RenderElementsIds::readConfig(){
	elementIdDrawnOption = DRAW_ALL;
	if(ui->radioButton_surface->isChecked())
		elementIdDrawnOption = DRAW_ONLY_SURFACE;
	else if(ui->radioButton_interior->isChecked())
		elementIdDrawnOption = DRAW_ONLY_INTERIOR;
	else if(ui->radioButton_selected->isChecked())
		elementIdDrawnOption = DRAW_ONLY_SELECTED;
	nElements = QtUtils::readUIntFromQText(ui->lineEdit_n_ids->text(),100);
	nElements = std::max(nElements,1u);
	nElements = std::min(nElements,10000u);
	useDepthStrategy = ui->radioButton_strategy_depth->isChecked();
	useShaderAcceleration = ui->radioButton_gpu->isChecked();
	elementIdDrawnOptionFacing = DRAW_ALL_FACES;
	if(ui->radioButton_front->isChecked())
		elementIdDrawnOptionFacing = DRAW_FRONT_FACES;
	else if(ui->radioButton_back->isChecked())
		elementIdDrawnOptionFacing = DRAW_BACK_FACES;
	else if(ui->radioButton_closer->isChecked())
		elementIdDrawnOptionFacing = DRAW_CLOSER_THAN;
	textColor.x = QtUtils::readFloatFromQText(ui->lineEdit_br->text(),0.0f);
	textColor.y = QtUtils::readFloatFromQText(ui->lineEdit_bg->text(),0.0f);
	textColor.z = QtUtils::readFloatFromQText(ui->lineEdit_bb->text(),0.0f);
	textColor.w = QtUtils::readFloatFromQText(ui->lineEdit_ba->text(),1.0f);
	textFillColor.x = QtUtils::readFloatFromQText(ui->lineEdit_sr->text(),1.0f);
	textFillColor.y = QtUtils::readFloatFromQText(ui->lineEdit_sg->text(),1.0f);
	textFillColor.z = QtUtils::readFloatFromQText(ui->lineEdit_sb->text(),0.0f);
	textFillColor.w = QtUtils::readFloatFromQText(ui->lineEdit_sa->text(),0.0f);
	fontSize = QtUtils::readFloatFromQText(ui->lineEdit_font_size->text(),1.0f);
	maxDistance = -QtUtils::readFloatFromQText(ui->lineEdit_maxDistance->text(),0.5f)-CAMERA_POSITION_Z;
}
#include <fstream>
bool RenderElementsIds::discard(vis::Element* e,bool ignoreSelection){
	if(elementIdDrawnOption == DRAW_ONLY_SURFACE)
		return !e->isAtSurface();
	if(!ignoreSelection && elementIdDrawnOption == DRAW_ONLY_SELECTED)
		return !e->isSelected();
	if(elementIdDrawnOption == DRAW_ONLY_INTERIOR)
		return e->isAtSurface();
	return false;
}
bool RenderElementsIds::discardSelection(vis::Element* e){
	return !e->isSelected();
}

void RenderElementsIds::renderIdsUsingShaders(Renderer*, RModel* rmodel){
	if(rmodel->positionDataBufferObject == RModel::NULL_BUFFER)
		return;// Create and set-up the vertex array object
	glUseProgram(theProgram);
	glm::vec2 viewPortSize = rmodel->getViewPortSize();
	ShaderUtils::setUniform(theProgram, "MVP",rmodel->getMVP());
	ShaderUtils::setUniform(theProgram, "VP",rmodel->getViewPortMatrix());
	ShaderUtils::setUniform(theProgram, "MV",rmodel->getMV());
	ShaderUtils::setUniform(theProgram, "orthoProjection",rmodel->isOrthoProjectionInUse());
	ShaderUtils::setUniform(theProgram, "viewPortOriginalSize",rmodel->getViewPortSize());

	ShaderUtils::setUniform(theProgram, "NormalMatrix",glm::mat3(rmodel->getMV()));
	ShaderUtils::setUniform(theProgram,"CellWidth", 2.0f/viewPortSize.x); //pass in the width of the cells
	ShaderUtils::setUniform(theProgram,"CellHeight", 2.0f/viewPortSize.y); //pass in the height of the cells
	ShaderUtils::setUniform(theProgram,"config.drawOption",elementIdDrawnOption);
	ShaderUtils::setUniform(theProgram,"config.drawOptionFacing",elementIdDrawnOptionFacing);
	ShaderUtils::setUniform(theProgram,"config.CameraPosition",rmodel->getCameraPosition());
	ShaderUtils::setUniform(theProgram,"config.elementsType",elementsType);
	ShaderUtils::setUniform(theProgram,"config.maxDistance",maxDistance);
	ShaderUtils::setUniform(theProgram,"config.idPercentSize",fontSize);

	ShaderUtils::setUniform(theProgram,"textColor",textColor);
	ShaderUtils::setUniform(theProgram,"textFillColor",textFillColor);


	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	ShaderUtils::setUniformTexture(theProgram,"Tex1",GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays

	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glEnableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	glEnableVertexAttribArray(PP_IDS_ATTRIBUTE); // Vertex selection
	glEnableVertexAttribArray(VERTEX_IDS_ATTRIBUTE); // Vertex selection
	glEnableVertexAttribArray(NORMAL_ATTRIBUTE); // Vertex selection

	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->positionDataBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexNormalDataBufferObject);
	glVertexAttribPointer( NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexFlagsDataBufferObject);
	glVertexAttribIPointer( VERTEX_FLAGS_ATTRIBUTE, 1, GL_UNSIGNED_INT, 0,
							(GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->polygonPolyhedronIdsBufferObject);
	glVertexAttribIPointer( PP_IDS_ATTRIBUTE, 1, GL_INT, 0,
							(GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vertexIdsBufferObject);
	glVertexAttribIPointer( VERTEX_IDS_ATTRIBUTE, 1, GL_INT, 0,
							(GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, rmodel->vertexFlagsAttribute.size() );
	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glDisableVertexAttribArray(VERTEX_FLAGS_ATTRIBUTE); // Vertex selection
	glDisableVertexAttribArray(PP_IDS_ATTRIBUTE); // Vertex selection
	glDisableVertexAttribArray(NORMAL_ATTRIBUTE); // Vertex selection
	glDisableVertexAttribArray(VERTEX_IDS_ATTRIBUTE); // Vertex selection
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void RenderElementsIds::initializeShader(){

	if(!compileShaderProgram()||!loadNumberTexture()){
		shaderOk = false;
		ui->radioButton_gpu->setDisabled(true);
	}
}

bool RenderElementsIds::compileShaderProgram(){
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("UI/RenderElementsIds/reids.vert");
	ShaderLoadingData geometryShaderData(GL_GEOMETRY_SHADER);
	geometryShaderData.addFile("UI/RenderElementsIds/reids.geom");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("UI/RenderElementsIds/reids.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(geometryShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData selectAttr = {VERTEX_FLAGS_ATTRIBUTE, "VertexFlags"};
	VertexAttributeBindingData vertexIdsAttr = {VERTEX_IDS_ATTRIBUTE, "VertexIds"};
	VertexAttributeBindingData ppIdsAttr = {PP_IDS_ATTRIBUTE, "PPIds"};
	VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
	VertexAttributeBindingData normalAttr = {NORMAL_ATTRIBUTE, "VertexNormal"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);
	attributeList.push_back(vertexIdsAttr);
	attributeList.push_back(ppIdsAttr);
	attributeList.push_back(selectAttr);
	attributeList.push_back(normalAttr);


	theProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(theProgram == ShaderUtils::FAIL_CREATING_PROGRAM)
		return false;
	return true;
}
bool RenderElementsIds::loadNumberTexture(){
	std::string texName = "UI/RenderElementsIds/numberTexture.raw";
	textureId = OpenGLUtils::uploadRaw2DTexture(texName,120u,18u,
											  GL_ALPHA,GL_ALPHA,GL_UNSIGNED_BYTE);
	if(!textureId){
		std::cerr << "Texture UI/RenderElementsIds/numberTexture.raw not found. "<<
					 "Drawing id's using GPU is disabled."<<std::endl;
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}
void RenderElementsIds::readGenericConfig(){

}

QWidget* RenderElementsIds::getGenericWidget(){
	return this;
}
