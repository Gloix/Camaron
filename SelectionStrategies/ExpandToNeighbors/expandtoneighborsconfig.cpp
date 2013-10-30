#include "expandtoneighborsconfig.h"
#include "ui_expandtoneighborsconfig.h"
#include "Utils/qtutils.h"
ExpandToNeighborsConfig::ExpandToNeighborsConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ExpandToNeighborsConfig)
{
	ui->setupUi(this);
}

ExpandToNeighborsConfig::~ExpandToNeighborsConfig()
{
	delete ui;
}
void ExpandToNeighborsConfig::readConfig(){
	dihedralAngle = QtUtils::readFloatFromQText(ui->lineEdit_dihedral->text(),0.0f);
	onlySurface = ui->radioButton_sur->isChecked();
	onlySurfaceWithAngle = ui->radioButton_sur_angle->isChecked();
}
