#include "phong1dconfig.h"
#include "ui_phong1dconfig.h"
#include "Utils/qtutils.h"
Phong1DConfig::Phong1DConfig(QWidget *parent) :
    BaseRendererConfig(parent),
	ui(new Ui::Phong1DConfig)
{
	ui->setupUi(this);
}

Phong1DConfig::~Phong1DConfig()
{
	delete ui;
}
void Phong1DConfig::readConfig(){
	material.Ka = QtUtils::readFloatFromQText(ui->lineEdit_ka->text(),1.0f);
	material.Kd = QtUtils::readFloatFromQText(ui->lineEdit_kd->text(),1.0f);
	material.Ks = QtUtils::readFloatFromQText(ui->lineEdit_ks->text(),1.0f);
	material.Shininess = QtUtils::readFloatFromQText(ui->lineEdit_shininess->text(),1.0f);
	light.La = QtUtils::readFloatFromQText(ui->lineEdit_la->text(),1.0f);
	light.Ld = QtUtils::readFloatFromQText(ui->lineEdit_ld->text(),1.0f);
	light.Ls = QtUtils::readFloatFromQText(ui->lineEdit_ls->text(),1.0f);

	light.Position.x = QtUtils::readFloatFromQText(ui->lineEdit_position_r->text(),0.0f);
	light.Position.y = QtUtils::readFloatFromQText(ui->lineEdit_position_g->text(),0.0f);
	light.Position.z = QtUtils::readFloatFromQText(ui->lineEdit_position_b->text(),0.0f);
	light.Position.w = 1.0f; // Light position in eye coords.
}
