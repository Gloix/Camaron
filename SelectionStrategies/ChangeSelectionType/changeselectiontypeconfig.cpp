#include "changeselectiontypeconfig.h"
#include "ui_changeselectiontypeconfig.h"
#include "Common/Constants.h"
ChangeSelectionTypeConfig::ChangeSelectionTypeConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ChangeSelectionTypeConfig)
{
	elementsType = vis::CONSTANTS::POLYGON;
	ui->setupUi(this);
	readConfig();
}

ChangeSelectionTypeConfig::~ChangeSelectionTypeConfig()
{
	delete ui;
}
void ChangeSelectionTypeConfig::readConfig(){
	if(ui->radio_polygon->isChecked())
		elementsType = vis::CONSTANTS::POLYGON;
	if(ui->radio_polyhedron->isChecked())
		elementsType = vis::CONSTANTS::POLYHEDRON;
	hint = ui->radioButton_all->isChecked();

}
