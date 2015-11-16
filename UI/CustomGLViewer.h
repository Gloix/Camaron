#ifndef CUSTOMGLVIEWER_H
#define CUSTOMGLVIEWER_H

#include "UI/RenderElementsIds/renderelementsids.h"
#include "UI/mainconfiguration.h"
#include "UI/rendererslist.h"
#include <QTimer>
#include <QGLWidget>
#include <glm/glm.hpp>
#include "Utils/keyboardstate.h"
#include "Utils/renderinglock.h"
class Selection;
class Renderer;
class RModel;
class FrameBufferObjectHandle;
class CustomGLViewer: public QGLWidget
{
		Q_OBJECT

	public:
		CustomGLViewer(RModel*,MainConfiguration&,
					   RenderersList&,QWidget *parent = 0);
		~CustomGLViewer();
		void resetCameraPosition();
		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		void setRenderer(Renderer*);
		Renderer* getRenderer();
		void setUpRenderer();
		void setSelection(Selection*);
		void refreshRenderedModel();
		void glewInitiated();
		void refreshHelpers();
		void setAxesVisible(bool value);

	public slots:
		void setXRotation(int angle);
		void setYRotation(int angle);
		void setZRotation(int angle);
		void applyRendererConfigChanges();
		void processRenderingQueue();
		void forceReRendering();

	signals:
		void xRotationChanged(int angle);
		void yRotationChanged(int angle);
		void zRotationChanged(int angle);
		void selectionBox(int,int,int,int);

	protected:
		void initializeGL();
		void paintEvent(QPaintEvent *);
		//void paintGL();
		void resizeGL(int width, int height);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent ( QWheelEvent * event );
		void keyPressEvent(QKeyEvent* event);
		void keyReleaseEvent(QKeyEvent* event);
		QSize minimumSize();
		QSize minimumSizeHint();
		QSize sizeHint();

	private:
//		QtLogo *logo;
		bool needsRefreshDrawing;
		bool boxSelect;
		glm::vec3 rot;
		glm::vec3 tra;
		glm::vec2 viewportSize;
		float zoom;
		QPoint lastPos;
		//from parent
		Renderer* renderer;
		RenderersList& secondaryRenderers;
		RModel* rmodel;
		Selection* selection;
		//CONFIG
		MainConfiguration& mainConfiguration;


		KeyboardState keyboardState;
		QImage screenshot;
		int dxSelectBox,dySelectBox;
		bool drawBox;
		bool drawIds;
		bool glewInitiatedState;
		int elementTypeIds;
		RenderingLock renderingLock;
		QTimer renderingTimer;
		RenderElementsIds *renderElementsIds;
		void paintOpenGl3DScene();
		void paintUsingRenderers();
		void paintSelectBoxOverlay(QPainter &painter);
		void paintScreenshotAsBackground(QPainter &painter);
		void paintFrameRateDisplay(QPainter &painter,float framerate);
		void saveRenderedModelAsQImage();
		//Average OIT
		GLuint weightedAverageProgram;
		FrameBufferObjectHandle* averageOITfboHandle;
		GLuint averageOITdepthStencilTextureHandle;
		GLuint averageOITmainTextureHandle;
		GLuint averageOITbackgroundTextureHandle;
		void initWeightedAverageShader();
		void resizeOITFBOTextures();
		const static GLuint VERTEX_POSITION = 0;
};
#endif // CUSTOMGLVIEWER_H
