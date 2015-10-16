#ifndef SHADINGEFFECTSRENDERCONFIG_H
#define SHADINGEFFECTSRENDERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/ShaderCommonFunctions/phongconfig.h"
#include "Rendering/Renderers/ShaderCommonFunctions/elementdrawnoptionconfig.h"
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class ShadingEffectsRenderConfig;
}

class ShadingEffectsRenderConfig : public BaseRendererConfig
{
	Q_OBJECT

	public:
	explicit ShadingEffectsRenderConfig(QWidget *parent = 0);
	~ShadingEffectsRenderConfig();
	void readConfig();
	//Config Properties
	PhongConfig phongConfig;
	ElementDrawnOptionConfig elementDrawnOptionConfig;
	bool translucentMode;
	bool inverseBackColor;
	private:
	Ui::ShadingEffectsRenderConfig *ui;
};

#endif // SHADINGEFFECTSRENDERCONFIG_H
