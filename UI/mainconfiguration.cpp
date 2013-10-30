#include "mainconfiguration.h"
#include "ui_mainconfiguration.h"
#include "Utils/qtutils.h"
#include "UI/genericconfigwidget.h"
#include <GL/glew.h>
MainConfiguration::MainConfiguration(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint),
	ui(new Ui::MainConfiguration)
{
	ui->setupUi(this);
	connect(ui->pushButton_apply,SIGNAL(clicked()),this,SLOT(readConfiguration()));
	readConfiguration();
}

MainConfiguration::~MainConfiguration()
{
	delete ui;
}

void MainConfiguration::readConfiguration(){
	//Renderer basics
	backgroundColor.x = QtUtils::readFloatFromQText(ui->lineEdit_bg_r->text(),1.0f);
	backgroundColor.y = QtUtils::readFloatFromQText(ui->lineEdit_bg_g->text(),1.0f);
	backgroundColor.z = QtUtils::readFloatFromQText(ui->lineEdit_bg_b->text(),1.0f);
	backgroundColor.w = QtUtils::readFloatFromQText(ui->lineEdit_bg_a->text(),1.0f);
	displayFrameRate = ui->checkBox_display_framerate->isChecked();
	frameRateFontColor.x = QtUtils::readFloatFromQText(ui->lineEdit_frate_r->text(),1.0f);
	frameRateFontColor.y = QtUtils::readFloatFromQText(ui->lineEdit_frate_g->text(),1.0f);
	frameRateFontColor.z = QtUtils::readFloatFromQText(ui->lineEdit_frate_b->text(),0.0f);
	frameRateFontColor.w = QtUtils::readFloatFromQText(ui->lineEdit_frate_a->text(),1.0f);
	frameRateFontBoxColor.x = QtUtils::readFloatFromQText(ui->lineEdit_fboxrate_r->text(),0.0f);
	frameRateFontBoxColor.y = QtUtils::readFloatFromQText(ui->lineEdit_fboxrate_g->text(),0.0f);
	frameRateFontBoxColor.z = QtUtils::readFloatFromQText(ui->lineEdit_fboxrate_b->text(),0.0f);
	frameRateFontBoxColor.w = QtUtils::readFloatFromQText(ui->lineEdit_fboxrate_a->text(),0.0f);
	useOrthoProjection = ui->radioButton_use_ortho->isChecked();
	perspectiveValue = QtUtils::readFloatFromQText(ui->lineEdit_persp->text(),5.0f);
	frameFontSize = QtUtils::readIntFromQText(ui->lineEdit_frameratefontsize->text(),10);
	//OpenGl Properties
	oitTechnique = MainConfiguration::USE_NONE;
	if(ui->radioButton_weighted_sum->isChecked())
		oitTechnique = MainConfiguration::USE_WEIGHTED_SUM;
	if(ui->radioButton_weighted_average->isChecked())
		oitTechnique = MainConfiguration::USE_WEIGHTED_AVERAGE;
	antialiasingEnabled = ui->checkBox_line_antialiasing->isChecked();
	transparencyEnabled = ui->checkBox_enable_transparency->isChecked();
	transparency2passEnabled = ui->radioButton_2_pass_blend->isChecked();
	alphatest_value = QtUtils::readFloatFromQText(ui->lineEdit_alphaTest->text(),0.85f);
	faceCullingEnabled = ui->checkBox_enable_culling->isChecked();
	backFaceIsCCW = ui->radioButton_ccw->isChecked();
	linewidth = QtUtils::readFloatFromQText(ui->lineEdit_line_width->text(),1.0f);
	if(linewidth<=0.1f){
		linewidth = 1.0f;
	}
	pointSize = QtUtils::readFloatFromQText(ui->lineEdit_point_size->text(),1.0f);
	if(pointSize<=0.1f){
		pointSize = 1.0f;
	}

	emit configurationUpdated();
}
void MainConfiguration::addNewTabWidget(GenericConfigWidget* newWidget){
	ui->tabWidget->addTab(newWidget->getGenericWidget(),QString::fromStdString(newWidget->getName()));
}
void MainConfiguration::weightedAverageFail(){
	ui->radioButton_no_weighted->setChecked(true);
	ui->widget__weighted->setDisabled(true);
}
