#include "mainrendererconfig.h"
#include "ui_mainrendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>

MainRendererConfig::MainRendererConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MainRendererConfig)
{
	wireFrameLineWidthM = 2;// 2 pixel
	drawWireFrame = true;
	ui->setupUi(this);
}

MainRendererConfig::~MainRendererConfig()
{
	delete ui;
}
void MainRendererConfig::readConfig(){
	float defaultColorValue = 1.0f;
	//Base model colors
	this->baseModelColors.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_br->text(),defaultColorValue);
	this->baseModelColors.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_bg->text(),defaultColorValue);
	this->baseModelColors.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_bb->text(),defaultColorValue);
	this->baseModelColors.w = QtUtils::readFloatFromQText(ui->lineEdit_MR_ba->text(),defaultColorValue);
	//Wire frame colors
	this->wireFrameColors.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_r->text(),defaultColorValue);
	this->wireFrameColors.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_g->text(),defaultColorValue);
	this->wireFrameColors.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_b->text(),defaultColorValue);
	this->wireFrameColors.w = QtUtils::readFloatFromQText(ui->lineEdit_MR_a->text(),defaultColorValue);
	this->wireFrameLineWidthM = QtUtils::readIntFromQText(ui->lineEdit_MR_w->text(),1);
	this->drawWireFrame =  Qt::Checked == this->ui->checkBox_MR_draw_w->checkState();

	//selected colors
	this->selectedElementColors.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_sr->text(),defaultColorValue);
	this->selectedElementColors.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_sg->text(),defaultColorValue);
	this->selectedElementColors.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_sb->text(),defaultColorValue);
	this->selectedElementColors.w = QtUtils::readFloatFromQText(ui->lineEdit_MR_sa->text(),defaultColorValue);
	elementDrawnOption = DRAW_ALL;
	if(ui->radioButton_surface->isChecked())
		elementDrawnOption = DRAW_ONLY_SURFACE;
	else if(ui->radioButton_interior->isChecked())
		elementDrawnOption = DRAW_ONLY_INTERIOR;
	else if(ui->radioButton_selected->isChecked())
		elementDrawnOption = DRAW_ONLY_SELECTED;
	else if(ui->radioButton_unselected->isChecked())
		elementDrawnOption = DRAW_ONLY_UNSELECTED;

	useDepthPeeling = ui->checkBox_depth_peeling->isChecked()&&
					  ui->checkBox_depth_peeling->isEnabled();
	depthPeelingLayers = QtUtils::readIntFromQText(ui->line_n_layers->text(),1);
}
void MainRendererConfig::depthPeelingNotAvailable(){
	ui->widget_depth_peeling->setEnabled(false);
}
