#include "vertexrendererconfig.h"
#include "ui_vertexrendererconfig.h"
#include "Utils/qtutils.h"
VertexRendererConfig::VertexRendererConfig(QWidget *parent) :
    BaseRendererConfig(parent),
	ui(new Ui::VertexRendererConfig)
{
	ui->setupUi(this);
}

VertexRendererConfig::~VertexRendererConfig()
{
	delete ui;
}
void VertexRendererConfig::applyConfig(){
	float defaultColorValue = 1.0f;
	//Base model colors
	this->baseVertexColors.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_br->text(),defaultColorValue);
	this->baseVertexColors.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_bg->text(),defaultColorValue);
	this->baseVertexColors.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_bb->text(),defaultColorValue);
	this->baseVertexColors.w = QtUtils::readFloatFromQText(ui->lineEdit_MR_ba->text(),defaultColorValue);
	//Selected model colors
	this->selectedVertexColors.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_sr->text(),defaultColorValue);
	this->selectedVertexColors.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_sg->text(),defaultColorValue);
	this->selectedVertexColors.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_sb->text(),defaultColorValue);
	this->selectedVertexColors.w = QtUtils::readFloatFromQText(ui->lineEdit_MR_sa->text(),defaultColorValue);
}
