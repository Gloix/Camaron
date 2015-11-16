#include "glassrendererconfig.h"
#include "ui_glassrendererconfig.h"
#include "Utils/qtutils.h"
GlassRendererConfig::GlassRendererConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	ui(new Ui::GlassRendererConfig)
{
	ui->setupUi(this);
	ui->widget_element_container->layout()->addWidget(&elementDrawConfig);
	ui->widget_phong_s_container->layout()->addWidget(&phong1DConfig);
}

GlassRendererConfig::~GlassRendererConfig()
{
	delete ui;
}
void GlassRendererConfig::readConfig(){
	phong1DConfig.readConfig();
	elementDrawConfig.readConfig();
	modelBaseColor.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_br->text(),1.0f);
	modelBaseColor.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_bg->text(),1.0f);
	modelBaseColor.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_bb->text(),1.0f);
	modelBaseColor.w = QtUtils::readFloatFromQText(ui->lineEdit_MR_ba->text(),1.0f);
	selectedBaseColor.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_sr->text(),1.0f);
	selectedBaseColor.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_sg->text(),1.0f);
	selectedBaseColor.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_sb->text(),1.0f);
	selectedBaseColor.w = QtUtils::readFloatFromQText(ui->lineEdit_MR_sa->text(),1.0f);
}
