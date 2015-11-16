#include "shadingeffectsrenderconfig.h"
#include "ui_shadingeffectsrenderconfig.h"
#include "Utils/qtutils.h"
ShadingEffectsRenderConfig::ShadingEffectsRenderConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	ui(new Ui::ShadingEffectsRenderConfig)
{
	ui->setupUi(this);
	ui->verticalLayout->addWidget(&phongConfig);
	ui->verticalLayout->addWidget(&elementDrawnOptionConfig);
}

ShadingEffectsRenderConfig::~ShadingEffectsRenderConfig()
{
	delete ui;
}

void ShadingEffectsRenderConfig::readConfig(){
	translucentMode = ui->checkBox_translucent_mode->isChecked();
	inverseBackColor = ui->checkBox_inverse_back_color->isChecked();
	phongConfig.readConfig();
	elementDrawnOptionConfig.readConfig();
}
