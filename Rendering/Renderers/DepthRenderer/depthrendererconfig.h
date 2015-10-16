#ifndef DEPTHRENDERERCONFIG_H
#define DEPTHRENDERERCONFIG_H

#include <GL/glew.h>
#include <QWidget>
#include <glm/glm.hpp>
#include <map>
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class DepthRendererConfig;
}

class DepthRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit DepthRendererConfig(QWidget *parent = 0);
		~DepthRendererConfig();
		void readConfig();
		glm::vec3 modelBaseColor;
		GLenum depthFunc;
	public slots:
		void changeDepthFunc(int index);
	private:
		Ui::DepthRendererConfig *ui;
		std::map<int,GLenum> enumIdsMap;
};

#endif // DEPTHRENDERERCONFIG_H
