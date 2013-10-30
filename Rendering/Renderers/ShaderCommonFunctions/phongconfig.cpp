#include "phongconfig.h"
#include "ui_phongconfig.h"
#include "Utils/qtutils.h"
PhongConfig::PhongConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhongConfig)
{
	ui->setupUi(this);
}

PhongConfig::~PhongConfig()
{
	delete ui;
}
void PhongConfig::readConfig(){
	material.Ka.x = QtUtils::readFloatFromQText(ui->lineEdit_ka_r->text(),1.0f);
	material.Ka.y = QtUtils::readFloatFromQText(ui->lineEdit_ka_g->text(),1.0f);
	material.Ka.z = QtUtils::readFloatFromQText(ui->lineEdit_ka_b->text(),1.0f);
	material.Kd.x = QtUtils::readFloatFromQText(ui->lineEdit_kd_r->text(),1.0f);
	material.Kd.y = QtUtils::readFloatFromQText(ui->lineEdit_kd_g->text(),1.0f);
	material.Kd.z = QtUtils::readFloatFromQText(ui->lineEdit_kd_b->text(),1.0f);
	material.Ks.x = QtUtils::readFloatFromQText(ui->lineEdit_ks_r->text(),1.0f);
	material.Ks.y = QtUtils::readFloatFromQText(ui->lineEdit_ks_g->text(),1.0f);
	material.Ks.z = QtUtils::readFloatFromQText(ui->lineEdit_ks_b->text(),1.0f);
	material.Shininess = QtUtils::readFloatFromQText(ui->lineEdit_shininess->text(),1.0f);
	light.La.x = QtUtils::readFloatFromQText(ui->lineEdit_la_r->text(),1.0f);
	light.La.y = QtUtils::readFloatFromQText(ui->lineEdit_la_g->text(),1.0f);
	light.La.z = QtUtils::readFloatFromQText(ui->lineEdit_la_b->text(),1.0f);
	light.Ld.x = QtUtils::readFloatFromQText(ui->lineEdit_ld_r->text(),1.0f);
	light.Ld.y = QtUtils::readFloatFromQText(ui->lineEdit_ld_g->text(),1.0f);
	light.Ld.z = QtUtils::readFloatFromQText(ui->lineEdit_ld_b->text(),1.0f);
	light.Ls.x = QtUtils::readFloatFromQText(ui->lineEdit_ls_r->text(),1.0f);
	light.Ls.y = QtUtils::readFloatFromQText(ui->lineEdit_ls_g->text(),1.0f);
	light.Ls.z = QtUtils::readFloatFromQText(ui->lineEdit_ls_b->text(),1.0f);

	light.Position.x = QtUtils::readFloatFromQText(ui->lineEdit_position_r->text(),0.0f);
	light.Position.y = QtUtils::readFloatFromQText(ui->lineEdit_position_g->text(),0.0f);
	light.Position.z = QtUtils::readFloatFromQText(ui->lineEdit_position_b->text(),0.0f);
	light.Position.w = 1.0f; // Light position in eye coords.
}
