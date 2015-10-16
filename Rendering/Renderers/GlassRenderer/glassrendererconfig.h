#ifndef GLASSRENDERERCONFIG_H
#define GLASSRENDERERCONFIG_H

#include <QWidget>
#include "Rendering/Renderers/ShaderCommonFunctions/elementdrawnoptionconfig.h"
#include "Rendering/Renderers/ShaderCommonFunctions/phong1dconfig.h"
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class GlassRendererConfig;
}

class GlassRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit GlassRendererConfig(QWidget *parent = 0);
		~GlassRendererConfig();
		void readConfig();
		Phong1DConfig phong1DConfig;
		ElementDrawnOptionConfig elementDrawConfig;
		glm::vec4 modelBaseColor;
		glm::vec4 selectedBaseColor;
		glm::vec4 materialInfo;

	private:
		Ui::GlassRendererConfig *ui;
};

#endif // GLASSRENDERERCONFIG_H
