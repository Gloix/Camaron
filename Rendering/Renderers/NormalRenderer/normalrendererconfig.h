#ifndef NORMALRENDERERCONFIG_H
#define NORMALRENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class NormalRendererConfig;
}

class NormalRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit NormalRendererConfig(QWidget *parent = 0);
		~NormalRendererConfig();
		glm::vec4 normalTailColor;
		glm::vec4 normalHeadColor;
		glm::vec4 wireframeColor;
		float normalLenght;
		int wireFrameOption;
		void readConfig();
	private:
		static const int NO_WIREFRAME = 0;
		static const int COMPLETE_WIREFRAME = 1;
		static const int SURFACE_WIREFRAME = 2;
		Ui::NormalRendererConfig *ui;
};

#endif // NORMALRENDERERCONFIG_H
