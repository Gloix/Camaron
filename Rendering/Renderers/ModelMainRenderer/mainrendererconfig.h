#ifndef MAINRENDERERCONFIG_H
#define MAINRENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class MainRendererConfig;
}

class MainRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit MainRendererConfig(QWidget *parent = 0);
		virtual ~MainRendererConfig();
		void readConfig();
		void depthPeelingNotAvailable();
		glm::vec4 wireFrameColors;
		glm::vec4 baseModelColors;
		glm::vec4 selectedElementColors;
		int wireFrameLineWidthM;
		bool drawWireFrame;
		bool useDepthPeeling;
		int depthPeelingLayers;
		int elementDrawnOption;
		static const int DRAW_ALL = 0;
		static const int DRAW_ONLY_SURFACE = 1;
		static const int DRAW_ONLY_INTERIOR = 2;
		static const int DRAW_ONLY_SELECTED = 3;
		static const int DRAW_ONLY_UNSELECTED = 4;
		
	private:
		Ui::MainRendererConfig *ui;
};

#endif // MAINRENDERERCONFIG_H
