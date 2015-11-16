#include "cartesiancoordinatesaxisconfig.h"
#include "ui_cartesiancoordinatesaxisconfig.h"
#include "Utils/qtutils.h"
CartesianCoordinatesAxisConfig::CartesianCoordinatesAxisConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	ui(new Ui::CartesianCoordinatesAxisConfig)
{
	ui->setupUi(this);
}

CartesianCoordinatesAxisConfig::~CartesianCoordinatesAxisConfig()
{
	delete ui;
}
void CartesianCoordinatesAxisConfig::readConfig(){
	drawAxix = ui->checkBox_axis_draw->isChecked();
	drawBoundingBox = ui->checkBox_bounding_draw->isChecked();
	drawExtremePoints = ui->checkBox_extreme_draw->isChecked();
	drawOriginPoint = ui->checkBox_origin_draw->isChecked();

	boundingBoxLineWidth = QtUtils::readFloatFromQText(ui->lineEdit_bounding_width->text(),1.0f);
	axisLineWidth= QtUtils::readFloatFromQText(ui->lineEdit_axis_width->text(),1.0f);
	extremePointSize = QtUtils::readFloatFromQText(ui->lineEdit_extreme_size->text(),1.0f);
	originPointSize = QtUtils::readFloatFromQText(ui->lineEdit_origin_size->text(),1.0f);

	QtUtilsReadQtLineEditsToGLMVEC3(xAxisColor,ui->lineEdit_xaxis);
	QtUtilsReadQtLineEditsToGLMVEC3(yAxisColor,ui->lineEdit_yaxis);
	QtUtilsReadQtLineEditsToGLMVEC3(zAxisColor,ui->lineEdit_zaxis);
	QtUtilsReadQtLineEditsToGLMVEC3(boundingBoxColor,ui->lineEdit_bounding);
	QtUtilsReadQtLineEditsToGLMVEC3(originPointColor,ui->lineEdit_origin);
	QtUtilsReadQtLineEditsToGLMVEC3(extremePointsColor,ui->lineEdit_extreme);

}
