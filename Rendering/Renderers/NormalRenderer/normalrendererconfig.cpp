#include "normalrendererconfig.h"
#include "ui_normalrendererconfig.h"
#include "Utils/qtutils.h"
NormalRendererConfig::NormalRendererConfig(QWidget *parent) :
    BaseRendererConfig(parent),
	ui(new Ui::NormalRendererConfig)
{
	ui->setupUi(this);
	readConfig();
}

NormalRendererConfig::~NormalRendererConfig()
{
	delete ui;
}

void NormalRendererConfig::readConfig(){

	normalTailColor.x = QtUtils::readFloatFromQText(ui->lineEdit_nt_r->text(),0.0f);
	normalTailColor.y = QtUtils::readFloatFromQText(ui->lineEdit_nt_g->text(),0.0f);
	normalTailColor.z = QtUtils::readFloatFromQText(ui->lineEdit_nt_b->text(),1.0f);
	normalTailColor.w = QtUtils::readFloatFromQText(ui->lineEdit_nt_a->text(),1.0f);
	normalHeadColor.x = QtUtils::readFloatFromQText(ui->lineEdit_nh_r->text(),0.0f);
	normalHeadColor.y = QtUtils::readFloatFromQText(ui->lineEdit_nh_g->text(),1.0f);
	normalHeadColor.z = QtUtils::readFloatFromQText(ui->lineEdit_nh_b->text(),0.0f);
	normalHeadColor.w = QtUtils::readFloatFromQText(ui->lineEdit_nh_a->text(),1.0f);
	wireframeColor.x = QtUtils::readFloatFromQText(ui->lineEdit_wf_r->text(),1.0f);
	wireframeColor.y = QtUtils::readFloatFromQText(ui->lineEdit_wf_g->text(),1.0f);
	wireframeColor.z = QtUtils::readFloatFromQText(ui->lineEdit_wf_b->text(),0.0f);
	wireframeColor.w = QtUtils::readFloatFromQText(ui->lineEdit_wf_a->text(),1.0f);
	normalLenght = QtUtils::readFloatFromQText(ui->lineEdit_n_length->text(),1.0f);

	if(ui->radioButton_wf_no->isChecked())
		wireFrameOption = NO_WIREFRAME;
	if(ui->radioButton_wf_complete->isChecked())
		wireFrameOption = COMPLETE_WIREFRAME;
	if(ui->radioButton_wf_surface->isChecked())
		wireFrameOption = SURFACE_WIREFRAME;

}
