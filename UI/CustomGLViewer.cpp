#include "Common/Constants.h"
#include "GL/glew.h"
//#include <QtGui>
#include <QMouseEvent>

#include <iostream>
#include <math.h>
#include "Rendering/Renderer.h"
#include "UI/CustomGLViewer.h"
#include "UI/mainconfiguration.h"
#include "Utils/matrixtransformationutils.h"
#include "Utils/shaderutils.h"
#include "Utils/openglutils.h"
#include "Utils/crosstimer.h"
#include "Utils/framebufferobjecthandle.h"
#include "glm/gtx/transform.hpp"
#include "Rendering/RModel/rmodel.h"
#include "Rendering/Renderers/SceneHelpers.h"
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif
#define MILLISECONDS_RENDERING_TIMER 17

CustomGLViewer::CustomGLViewer(RModel* renModel,MainConfiguration& m,
							   RenderersList& renderers,QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
	  secondaryRenderers(renderers),
	  mainConfiguration(m)
{
	averageOITfboHandle = 0;
	glewInitiatedState = false;
	this->rmodel = renModel;
	resetCameraPosition();
	this->setFocusPolicy(Qt::ClickFocus);
	this->rmodel->setViewMatrix(glm::lookAt( glm::vec3(0.0,0.0,5.0),
											 glm::vec3(0.0,0.0,0.0),
											 glm::vec3(0.0,1.0,0.0)));
	this->renderer = (Renderer*)0;
	needsRefreshDrawing = false;
	boxSelect = false;
	drawBox = false;
	drawIds = false;
	elementTypeIds = vis::CONSTANTS::POLYGON;
	setAutoFillBackground(false);
	setAutoBufferSwap(false);
	renderingTimer.setSingleShot(false);
	connect(&renderingTimer, SIGNAL(timeout()), this, SLOT(processRenderingQueue()));
	renderingTimer.start(MILLISECONDS_RENDERING_TIMER);
	renderElementsIds = new RenderElementsIds(this);
	mainConfiguration.addNewTabWidget(renderElementsIds);
}
void CustomGLViewer::resetCameraPosition(){
	rot = glm::vec3(0.0f,0.0f,0.0f);
	tra = glm::vec3(0.0f,0.0f,50.0f);
	zoom = 1.0f;
}

CustomGLViewer::~CustomGLViewer()
{
	delete averageOITfboHandle;
	if(averageOITmainTextureHandle)
		glDeleteTextures(1,&averageOITmainTextureHandle);
	if(averageOITbackgroundTextureHandle)
		glDeleteTextures(1,&averageOITbackgroundTextureHandle);
	if(averageOITdepthStencilTextureHandle)
		glDeleteTextures(1,&averageOITdepthStencilTextureHandle);
	SceneHelpers::reset();
}
void CustomGLViewer::setRenderer(Renderer * r){
	this->renderer = r;
	setUpRenderer();
}
Renderer* CustomGLViewer::getRenderer(){
	return renderer;
}

void CustomGLViewer::setUpRenderer(){
	if(renderer){
		renderer->rendererSelected(this->rmodel);
	}
}

QSize CustomGLViewer::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize CustomGLViewer::sizeHint() const
{
	return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void CustomGLViewer::setXRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != rot.x) {
		rot.x = angle;
		emit xRotationChanged(angle);
		needsRefreshDrawing = true;
	}
}
void CustomGLViewer::setYRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != rot.y) {
		rot.y = angle;
		emit yRotationChanged(angle);
		needsRefreshDrawing = true;
	}
}

void CustomGLViewer::setZRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != rot.z) {
		rot.z = angle;
		emit zRotationChanged(angle);
		needsRefreshDrawing = true;
	}
}
void CustomGLViewer::initializeGL()
{
	glEnable(GL_DEPTH_TEST);
	if(mainConfiguration.transparencyEnabled){
		glDisable(GL_ALPHA_TEST);
		glEnable( GL_BLEND );
		if(glewInitiatedState)
			glBlendEquation(GL_FUNC_ADD);
		glBlendFunc (GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	}else
		glDisable( GL_BLEND );
	if(mainConfiguration.faceCullingEnabled){
		glEnable(GL_CULL_FACE);
		if(!mainConfiguration.backFaceIsCCW)
			glFrontFace(GL_CCW);
		else
			glFrontFace(GL_CW);
	}
	else{
		glDisable(GL_CULL_FACE);
	}
	glLineWidth(mainConfiguration.linewidth);
	glPointSize(mainConfiguration.pointSize);
	if(mainConfiguration.antialiasingEnabled){
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POINT_SMOOTH);
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
	}else{
		glDisable( GL_LINE_SMOOTH );
		glDisable( GL_POINT_SMOOTH );
	}
	glClearColor(mainConfiguration.backgroundColor.x,
				 mainConfiguration.backgroundColor.y,
				 mainConfiguration.backgroundColor.z,
				 mainConfiguration.backgroundColor.w);
	//glEnable(GL_DEPTH_TEST);
}

void CustomGLViewer::paintUsingRenderers(){
	this->renderer->draw(this->rmodel);
	std::vector<Renderer*>& second = secondaryRenderers.getRenderers();
	for(std::vector<Renderer*>::size_type i = 0;i<second.size();i++)
		if(second[i]!=renderer)second[i]->draw(this->rmodel);

	SceneHelpers::getInstance()->drawHelpers(this->rmodel);
}
#include <fstream>
void CustomGLViewer::paintOpenGl3DScene(){
	if(this->renderer){
		float translationIncrement = 0.02;
		glm::vec3 finalTranslation = tra*translationIncrement;
		finalTranslation.z = tra.z * 0.1f;
		this->rmodel->setRotation(rot*(PI/(180.0f*16.0f)));
		this->rmodel->setTranslation(finalTranslation);
		this->rmodel->setScale(zoom);
		if(mainConfiguration.transparencyEnabled&&
				mainConfiguration.transparency2passEnabled){
			//2 pass using alpha testing but without OIT
			if(mainConfiguration.oitTechnique == MainConfiguration::USE_NONE){//draw opaque
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GEQUAL,mainConfiguration.alphatest_value);
				paintUsingRenderers();
				//draw transparent
				if(glewInitiatedState)
					glBlendEquation(GL_FUNC_ADD);
				glAlphaFunc(GL_LESS,mainConfiguration.alphatest_value);
				glDepthMask(false);
				paintUsingRenderers();
				glDisable(GL_ALPHA_TEST);
				glDepthMask(true);
			}
			else {

				//draw opaque
				//ENABLE FBO
				averageOITfboHandle->bind();

				averageOITfboHandle->attachTexture(TextureBufferData(averageOITbackgroundTextureHandle,
																	 GL_COLOR_ATTACHMENT0,
																	 GL_TEXTURE_2D),true);
				averageOITfboHandle->attachTexture(TextureBufferData(averageOITdepthStencilTextureHandle,
																	 GL_DEPTH_STENCIL_ATTACHMENT,
																	 GL_TEXTURE_2D),true);

				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);

				//DRAW OPAQUE
				glClearColor(mainConfiguration.backgroundColor.x,
							 mainConfiguration.backgroundColor.y,
							 mainConfiguration.backgroundColor.z,
							 mainConfiguration.backgroundColor.w);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				glEnable(GL_ALPHA_TEST);
				glEnable(GL_DEPTH_TEST);
				glAlphaFunc(GL_GEQUAL,mainConfiguration.alphatest_value);
				paintUsingRenderers();
				//DRAW TRANSLUCENT
				//change texture
				averageOITfboHandle->attachTexture(TextureBufferData(averageOITmainTextureHandle,
																	 GL_COLOR_ATTACHMENT0,
																	 GL_TEXTURE_2D),true);
				glEnable(GL_STENCIL_TEST);
				glBlendEquation(GL_FUNC_ADD);
				glClearStencil(0);
				glClearColor(0.0f,0.0f,0.0f,0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS,1,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);//GL_KEEP
				glAlphaFunc(GL_LESS,mainConfiguration.alphatest_value);
				glDepthMask(false);
				glStencilMask(0xFF);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE,GL_ONE);

				paintUsingRenderers();
				glDepthMask(true);
				glDisable(GL_ALPHA_TEST);
				averageOITfboHandle->attachTexture(TextureBufferData(0,GL_COLOR_ATTACHMENT0,
																	 GL_TEXTURE_2D),true);
				averageOITfboHandle->attachTexture(TextureBufferData(0,GL_DEPTH_STENCIL_ATTACHMENT,
																	 GL_TEXTURE_2D),true);
				averageOITfboHandle->unbind();
				//get to file
				/*char* texContent = new char[(int)(viewportSize.x*viewportSize.y)*4];
				char* texContentSoloStenci = new char[(int)(viewportSize.x*viewportSize.y)];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,averageOITdepthStencilTextureHandle);
				if(OpenGLUtils::printOpenGLError())
					std::cout << "NO HAY ERROR AQUI222"<<std::endl;
				glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_STENCIL_INDEX);
				glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,texContent);
				//glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,texContent);
				if(OpenGLUtils::printOpenGLError())
					std::cout << "NO HAY ERROR AQUI"<<std::endl;
				std::cout << "width: " << viewportSize.x << ", height: "<<viewportSize.y<<std::endl;
				std::fstream f("asdafsf.raw",std::fstream::out | std::fstream::binary |
							   std::fstream::trunc);
				unsigned char* otropuntero = (unsigned char*)texContent;
				unsigned char* otrotexContentSoloStenci = (unsigned char*)texContentSoloStenci;

				//std::cout << "-----------------" << std::endl;
				for(int s = 0;s<viewportSize.y;s++){
					for(int t = 0;t<viewportSize.x;t++){
						*otrotexContentSoloStenci = ((*otropuntero)+1)*10;
						//std::cout << (int)*otrotexContentSoloStenci << "\t";
						++otropuntero;++otropuntero;++otropuntero;++otropuntero;
						++otrotexContentSoloStenci;

					}
					//std::cout << std::endl;
				}
				//std::cout << "-----------------" << std::endl;
				f.write(texContentSoloStenci,(int)(viewportSize.x*viewportSize.y));
				f.close();//
				delete[] texContent;
				delete[] texContentSoloStenci;
				//end/*/
				glDisable(GL_STENCIL_TEST);
				glClearColor(mainConfiguration.backgroundColor.x,
							 mainConfiguration.backgroundColor.y,
							 mainConfiguration.backgroundColor.z,
							 mainConfiguration.backgroundColor.w);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBlendEquation( GL_FUNC_ADD);
				glUseProgram(weightedAverageProgram);
				ShaderUtils::setUniform(weightedAverageProgram,"useWeightedAverage",mainConfiguration.oitTechnique == MainConfiguration::USE_WEIGHTED_AVERAGE);
				glEnable(GL_TEXTURE_2D);
				ShaderUtils::setUniformTexture(weightedAverageProgram,"TexColorsAlpha",0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, averageOITmainTextureHandle);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				ShaderUtils::setUniformTexture(weightedAverageProgram,"TexColorsDepth",1);
				glActiveTexture(GL_TEXTURE0+1);
				glBindTexture(GL_TEXTURE_2D, averageOITdepthStencilTextureHandle);
				glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_STENCIL_INDEX);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				ShaderUtils::setUniformTexture(weightedAverageProgram,"TexBackground",2);
				glActiveTexture(GL_TEXTURE0+2);
				glBindTexture(GL_TEXTURE_2D, averageOITbackgroundTextureHandle);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				float tripos[12] = {-1,-1,1,-1,-1,1,
								   1,-1,1,1,-1,1};

				glEnableClientState( GL_VERTEX_ARRAY );
				glVertexPointer(2,GL_FLOAT,0,&tripos[0]);

				glDrawArrays(GL_TRIANGLES,0,12);

				glDisableClientState(GL_VERTEX_ARRAY );
				glUseProgram(0);
				glDisable(GL_TEXTURE_2D);
			}
		}

		else{
			if(!mainConfiguration.transparencyEnabled ||
					mainConfiguration.oitTechnique == MainConfiguration::USE_NONE){
				if(!mainConfiguration.transparencyEnabled)
					glDisable(GL_BLEND);
				paintUsingRenderers();
			}
			else{
				averageOITfboHandle->bind();
				averageOITfboHandle->attachTexture(TextureBufferData(averageOITbackgroundTextureHandle,
																	 GL_COLOR_ATTACHMENT0,
																	 GL_TEXTURE_2D),true);
				averageOITfboHandle->attachTexture(TextureBufferData(averageOITdepthStencilTextureHandle,
																	 GL_DEPTH_STENCIL_ATTACHMENT,
																	 GL_TEXTURE_2D),true);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);
				glClearColor(mainConfiguration.backgroundColor.x,
							 mainConfiguration.backgroundColor.y,
							 mainConfiguration.backgroundColor.z,
							 mainConfiguration.backgroundColor.w);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				averageOITfboHandle->attachTexture(TextureBufferData(averageOITmainTextureHandle,
																	 GL_COLOR_ATTACHMENT0,
																	 GL_TEXTURE_2D),true);
				glEnable(GL_STENCIL_TEST);
				glDisable(GL_DEPTH_TEST);
				glBlendEquation(GL_FUNC_ADD);
				glClearStencil(0);
				glClearColor(0.0f,0.0f,0.0f,0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS,1,0xFF);
				glStencilOp(GL_KEEP,GL_INCR,GL_INCR);//GL_KEEP
				glStencilMask(0xFF);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE,GL_ONE);
				paintUsingRenderers();
				averageOITfboHandle->attachTexture(TextureBufferData(0,GL_COLOR_ATTACHMENT0,
																	 GL_TEXTURE_2D),true);
				averageOITfboHandle->attachTexture(TextureBufferData(0,GL_DEPTH_STENCIL_ATTACHMENT,
																	 GL_TEXTURE_2D),true);
				averageOITfboHandle->unbind();
				glDisable(GL_STENCIL_TEST);
				glClearColor(mainConfiguration.backgroundColor.x,
							 mainConfiguration.backgroundColor.y,
							 mainConfiguration.backgroundColor.z,
							 mainConfiguration.backgroundColor.w);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBlendEquation( GL_FUNC_ADD);
				/*//get to file
				char* texContent = new char[(int)(viewportSize.x*viewportSize.y)*4];
				char* texContentSoloStenci = new char[(int)(viewportSize.x*viewportSize.y)];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,averageOITdepthStencilTextureHandle);
				if(OpenGLUtils::printOpenGLError())
					std::cout << "NO HAY ERROR AQUI222"<<std::endl;
				glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_STENCIL_INDEX);
				glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,texContent);
				//glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,texContent);
				if(OpenGLUtils::printOpenGLError())
					std::cout << "NO HAY ERROR AQUI"<<std::endl;
				std::cout << "width: " << viewportSize.x << ", height: "<<viewportSize.y<<std::endl;
				std::fstream f("asdafsf.raw",std::fstream::out | std::fstream::binary |
							   std::fstream::trunc);
				unsigned char* otropuntero = (unsigned char*)texContent;
				unsigned char* otrotexContentSoloStenci = (unsigned char*)texContentSoloStenci;

				//std::cout << "-----------------" << std::endl;
				for(int s = 0;s<viewportSize.y;s++){
					for(int t = 0;t<viewportSize.x;t++){
						*otrotexContentSoloStenci = ((*otropuntero)+1)*10;
						//std::cout << (int)*otrotexContentSoloStenci << "\t";
						++otropuntero;++otropuntero;++otropuntero;++otropuntero;
						++otrotexContentSoloStenci;

					}
					//std::cout << std::endl;
				}
				//std::cout << "-----------------" << std::endl;
				f.write(texContentSoloStenci,(int)(viewportSize.x*viewportSize.y));
				f.close();//
				delete[] texContent;
				delete[] texContentSoloStenci;
				//end/*/
				glUseProgram(weightedAverageProgram);
				ShaderUtils::setUniform(weightedAverageProgram,"useWeightedAverage",mainConfiguration.oitTechnique == MainConfiguration::USE_WEIGHTED_AVERAGE);
				glEnable(GL_TEXTURE_2D);
				ShaderUtils::setUniformTexture(weightedAverageProgram,"TexColorsAlpha",0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, averageOITmainTextureHandle);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				ShaderUtils::setUniformTexture(weightedAverageProgram,"TexColorsDepth",1);
				glActiveTexture(GL_TEXTURE0+1);
				glBindTexture(GL_TEXTURE_2D, averageOITdepthStencilTextureHandle);
				glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_STENCIL_INDEX);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				ShaderUtils::setUniformTexture(weightedAverageProgram,"TexBackground",2);
				glActiveTexture(GL_TEXTURE0+2);
				glBindTexture(GL_TEXTURE_2D, averageOITbackgroundTextureHandle);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				float tripos[12] = {-1,-1,1,-1,-1,1,
								   1,-1,1,1,-1,1};

				glEnableClientState( GL_VERTEX_ARRAY );
				glVertexPointer(2,GL_FLOAT,0,&tripos[0]);

				glDrawArrays(GL_TRIANGLES,0,12);

				glDisableClientState(GL_VERTEX_ARRAY );
				glUseProgram(0);
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_DEPTH_TEST);
				glDepthMask(true);
			}
		}

	}
	else
		std::cout << "There is no renderer loaded to draw." << std::endl;
}

void CustomGLViewer::paintSelectBoxOverlay(QPainter &painter){
	if(drawBox){
		CrossTimer crr;
		//painter.setPen(QColor(255,190,0,190));
		painter.setPen(QColor(34,255,0,190));
		//painter.setBrush(QBrush(QColor(255,255,0,100)));
		painter.setBrush(QBrush(QColor(0,255,0,100)));
		painter.drawRect(QRect(lastPos.x(),lastPos.y(),
							   dxSelectBox,dySelectBox));
		drawBox = false;
		std::cout << "CustomGLViewer::paintSelectBoxOverlay: " << crr.getTranscurredSeconds() << " seconds" << std::endl;
	}
}

void CustomGLViewer::paintScreenshotAsBackground(QPainter &painter){
	painter.drawImage(0,0,screenshot);
}
void CustomGLViewer::paintFrameRateDisplay(QPainter &painter,float framerate){
	if(!mainConfiguration.displayFrameRate)
		return;
	painter.setBrush(QBrush(QColor((int)(255*mainConfiguration.frameRateFontBoxColor.x),
								   (int)(255*mainConfiguration.frameRateFontBoxColor.y),
								   (int)(255*mainConfiguration.frameRateFontBoxColor.z),
								   (int)(255*mainConfiguration.frameRateFontBoxColor.w))));
	painter.setPen(QColor((int)(255*mainConfiguration.frameRateFontColor.x),
						  (int)(255*mainConfiguration.frameRateFontColor.y),
						  (int)(255*mainConfiguration.frameRateFontColor.z),
						  (int)(255*mainConfiguration.frameRateFontColor.w)));

	//painter.drawRect(QRect(100,100,20,20));
	QFont font = painter.font();
	int newPointSize = mainConfiguration.frameFontSize;
	font.setPointSize(newPointSize);
	painter.setFont(font);
	framerate = std::min(framerate,9999.99f);
	int framerateint = framerate*100;
	QString fixedSizeString("2000.00");
	int textHeight = QFontMetrics( font ).height();
	int textWidth = QFontMetrics( font ).width(fixedSizeString);

	glm::vec2 pos(width()-textWidth-5,5);
	int extra = 2;
	QRect rect(pos.x,pos.y,textWidth,textHeight);
	painter.drawRect(rect.x() - extra,
					 rect.y() - extra,
					 rect.width() + extra + extra,
					 rect.height() + extra + extra);
	painter.drawText(rect,QString::number(framerateint/100.0f));
}

void CustomGLViewer::refreshHelpers(){
	SceneHelpers::reset();
}

#include "Utils/openglutils.h"
void CustomGLViewer::paintEvent(QPaintEvent *)
{
	CrossTimer crr;
	initializeGL();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_MULTISAMPLE);

	glLoadIdentity();
	if(!drawBox)
		paintOpenGl3DScene();
	QPainter painter(this);
	glDisable(GL_DEPTH_TEST);
	painter.setRenderHint(QPainter::Antialiasing);
	if(drawIds){
		renderElementsIds->setElementsType(elementTypeIds);
		renderElementsIds->renderIds(renderer,rmodel,selection,painter);
	}
	if(drawBox)
		paintScreenshotAsBackground(painter);
	else{
		float secsTot = crr.getTranscurredSeconds();
		paintFrameRateDisplay(painter,1.0f/secsTot);
		// std::cout << "CustomGLViewer::paintEvent: " << 1.0f/secsTot << " fps" << std::endl;
	}
	paintSelectBoxOverlay(painter);
	painter.end();

	//glFinish();
	swapBuffers();
	//std::cout << "CustomGLViewer::paintEvent: " << 1.0f/secsTot << " fps" << std::endl;
}
void CustomGLViewer::applyRendererConfigChanges(){
	if(renderer)
		renderer->applyConfigChanges(rmodel);
	this->repaint();
}

void CustomGLViewer::resizeGL(int width, int height)
{
	float near_cl = -150.0f;
	float far_cl = 150.0f;
	glViewport(0,0,width,height);
	viewportSize = glm::vec2(width,height);
	rmodel->setViewPortSize(glm::vec2(width,height),
							glm::vec2(near_cl,far_cl));
	if(averageOITfboHandle){
		resizeOITFBOTextures();
		averageOITfboHandle->setNewSize(width,height);
	}
}
void CustomGLViewer::saveRenderedModelAsQImage(){
	glReadBuffer(GL_FRONT);
	screenshot = grabFrameBuffer( );
	//glReadBuffer(GL_BACK);
}
void CustomGLViewer::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	dxSelectBox = 0;
	dySelectBox = 0;
	if(keyboardState.isKeyPressed(Qt::Key_Shift)){
		boxSelect = true;
		saveRenderedModelAsQImage();
	}
	else
		boxSelect = false;
}
void CustomGLViewer::mouseReleaseEvent(QMouseEvent *)
{
	if(drawBox || boxSelect){
		drawBox= false;
		boxSelect = false;
		emit selectionBox(lastPos.x(),lastPos.y(), lastPos.x()+dxSelectBox, lastPos.y()+dySelectBox);
		forceReRendering();
	}
}
void CustomGLViewer::wheelEvent ( QWheelEvent * event ){
	//std::cout << "RUEDA: " << event->type() << ", es QEvent::Wheel?= "<< QEvent::Wheel << std::endl;
	//std::cout << "RUEDA (x,Y): " << event->x() << ", " << event->y() << ", delta = " << event->delta() << std::endl;
	bool up = event->delta() > 0;
	float zoomIncrement = 1.1;
	if(up)
		this->zoom *= zoomIncrement;
	else
		this->zoom /= 1.1f;
	forceReRendering();
}

void CustomGLViewer::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();
	if (event->buttons() & Qt::LeftButton) {
		if(boxSelect){
			drawBox = true;
			dxSelectBox = dx;
			dySelectBox = dy;
			forceReRendering();
			return;
		}
		setXRotation(rot.x + 8 * dy);
		setYRotation(rot.y + 8 * dx);

	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(rot.x + 8 * dy);
		setZRotation(rot.z + 8 * dx);
	}
	if(needsRefreshDrawing){
		forceReRendering();
		needsRefreshDrawing = false;
	}
	lastPos = event->pos();
}

void CustomGLViewer::keyPressEvent(QKeyEvent* event){
	if ( !event->isAutoRepeat() ) {
		keyboardState.keyPressed(event->key());
		std::cout << "key event in board: " << event->key() << std::endl;
	}
	if(event->key()==Qt::Key_I){
		elementTypeIds = vis::CONSTANTS::POLYGON;
		drawIds = true;
		forceReRendering();
		return;
	}
	if(event->key()==Qt::Key_O){
		elementTypeIds = vis::CONSTANTS::POLYHEDRON;
		drawIds = true;
		forceReRendering();
		return;
	}
	if(event->key()==Qt::Key_U){
		elementTypeIds = vis::CONSTANTS::VERTEX;
		drawIds = true;
		forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Down && keyboardState.isKeyPressed(Qt::Key_Control)){
		tra.z ++;
		forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Up && keyboardState.isKeyPressed(Qt::Key_Control)){
		tra.z --;
		forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Down){
		tra.y ++;
		forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Up){
		tra.y --;
		forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Left){
		tra.x ++;
		forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Right){
		tra.x --;
		forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Space){
		resetCameraPosition();
		forceReRendering();
		return;
	}
}
void CustomGLViewer::keyReleaseEvent(QKeyEvent* event){
	if (!event->isAutoRepeat() ) {
		std::cout << "key event release in board: " << event->key() << std::endl;
		keyboardState.keyRelesed(event->key());
	}
	if(event->key()==Qt::Key_I || event->key()==Qt::Key_O || event->key()==Qt::Key_U){
		drawIds = false;
		forceReRendering();
	}

}

void CustomGLViewer::processRenderingQueue(){
	renderingTimer.stop();
	if(renderingLock.needsRendering()){
		repaint();
		updateGL();
	}
	renderingTimer.start(MILLISECONDS_RENDERING_TIMER);
}

void CustomGLViewer::forceReRendering(){
	renderingLock.setRenderingNeed(true);
}
void CustomGLViewer::setSelection(Selection* s){
	selection = s;
}
void CustomGLViewer::glewInitiated(){
	renderElementsIds->initializeShader();
	glewInitiatedState = true;
	initWeightedAverageShader();
}
void CustomGLViewer::initWeightedAverageShader(){
	ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
	vertexShaderData.addFile("UI/CustomGLViewer/WeightedAverageShader.vert");
	ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
	fragmentShaderData.addFile("UI/CustomGLViewer/WeightedAverageShader.frag");

	std::vector<ShaderLoadingData> shaderList;
	shaderList.push_back(vertexShaderData);
	shaderList.push_back(fragmentShaderData);


	VertexAttributeBindingData positionAttr = {VERTEX_POSITION, "VertexPosition"};
	std::vector<VertexAttributeBindingData> attributeList;
	attributeList.push_back(positionAttr);

	weightedAverageProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
	if(weightedAverageProgram == ShaderUtils::FAIL_CREATING_PROGRAM){
		weightedAverageProgram = 0;
		mainConfiguration.weightedAverageFail();
	}else{
		averageOITmainTextureHandle = OpenGLUtils::uploadRaw2DTexture(0,viewportSize.x,viewportSize.y,GL_RGBA32F,GL_RGBA,GL_UNSIGNED_BYTE);
		averageOITbackgroundTextureHandle = OpenGLUtils::uploadRaw2DTexture(0,viewportSize.x,viewportSize.y,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
		TextureBufferData colorsBuffer = {averageOITmainTextureHandle,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D};
		averageOITfboHandle = new FrameBufferObjectHandle(viewportSize.x,
														  viewportSize.y,
														  colorsBuffer);
		averageOITdepthStencilTextureHandle = averageOITfboHandle->getDepthStencilTextureHandle();

	}
}
void CustomGLViewer::resizeOITFBOTextures(){
	//init FBO
	OpenGLUtils::deleteTexture(averageOITmainTextureHandle);
	OpenGLUtils::deleteTexture(averageOITbackgroundTextureHandle);
	OpenGLUtils::deleteTexture(averageOITdepthStencilTextureHandle);
	averageOITmainTextureHandle = OpenGLUtils::uploadRaw2DTexture(0,viewportSize.x,viewportSize.y,GL_RGBA32F,GL_RGBA,GL_UNSIGNED_BYTE);
	averageOITbackgroundTextureHandle = OpenGLUtils::uploadRaw2DTexture(0,viewportSize.x,viewportSize.y,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
	averageOITdepthStencilTextureHandle = OpenGLUtils::uploadRaw2DTexture(0,viewportSize.x,viewportSize.y,
																		  GL_DEPTH24_STENCIL8,
																		  GL_DEPTH_STENCIL,
																		  GL_UNSIGNED_INT_24_8);
}

void CustomGLViewer::setAxesVisible(bool value){
	SceneHelpers::getInstance()->setAxesVisible(value);

}
