#ifndef VERTEXRENDERERCONFIG_H
#define VERTEXRENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class VertexRendererConfig;
}

class VertexRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit VertexRendererConfig(QWidget *parent = 0);
		virtual ~VertexRendererConfig();
		void applyConfig();
		glm::vec4 baseVertexColors;
		glm::vec4 selectedVertexColors;
	private:
		Ui::VertexRendererConfig *ui;
};

#endif // VERTEXRENDERERCONFIG_H
