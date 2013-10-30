#include "mouseselectionconfig.h"
#include "ui_mouseselectionconfig.h"
#include "Common/Constants.h"
#include "Utils/qtutils.h"
MouseSelectionConfig::MouseSelectionConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MouseSelectionConfig)
{
	elementsType = vis::CONSTANTS::POLYGON;
	ui->setupUi(this);
	readConfig();
}

MouseSelectionConfig::~MouseSelectionConfig()
{
	delete ui;
}
void MouseSelectionConfig::readConfig(){
	if(ui->radio_polygon->isChecked())
		elementsType = vis::CONSTANTS::POLYGON;
	if(ui->radio_polyhedron->isChecked())
		elementsType = vis::CONSTANTS::POLYHEDRON;
	if(ui->radio_new_selection->isChecked())
		selectionType = MouseSelectionConfig::NEW_SELECTION;
	if(ui->radio_join_selection->isChecked())
		selectionType = MouseSelectionConfig::JOIN_SELECTION;
	if(ui->radio_intersect_selection->isChecked())
		selectionType = MouseSelectionConfig::INTERSECT_SELECTION;
	if(ui->radio_invert->isChecked())
		selectionType = MouseSelectionConfig::INVERT_SELECTION;
	onlyInterior = ui->radio_interior->isChecked();
	onlySurface = ui->radio_surface->isChecked();
	onlyFront = ui->radio_ccw->isChecked();
	onlyBack = ui->radio_cw->isChecked();
//	closestToCamera = ui->radio_closest->isChecked();
	closestToCamera = false;
	//pixelTolerance = QtUtils::readFloatFromQText(ui->lineEdit_pTolerance->text(),2.0f);
	pixelTolerance = 2.0f;
}
