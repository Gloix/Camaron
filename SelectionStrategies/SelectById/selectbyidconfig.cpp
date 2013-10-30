#include "selectbyidconfig.h"
#include "ui_selectbyidconfig.h"
#include "Common/Constants.h"
#include "Utils/qtutils.h"
SelectByIdConfig::SelectByIdConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SelectByIdConfig)
{
	ui->setupUi(this);
	readConfig();
}

SelectByIdConfig::~SelectByIdConfig()
{
	delete ui;
}
void SelectByIdConfig::readConfig(){
	if(ui->radioButton_polygon->isChecked())
		elementsType = vis::CONSTANTS::POLYGON;
	if(ui->radioButton_polyhedron->isChecked())
		elementsType = vis::CONSTANTS::POLYHEDRON;
	if(ui->radio_new_selection_2->isChecked())
		selectionType = SelectByIdConfig::NEW_SELECTION;
	if(ui->radio_join_selection_2->isChecked())
		selectionType = SelectByIdConfig::JOIN_SELECTION;
	if(ui->radio_intersect_selection_2->isChecked())
		selectionType = SelectByIdConfig::INTERSECT_SELECTION;
	minId = QtUtils::readIntFromQText(ui->lineEdit_id_from->text(),0);
	maxId = QtUtils::readIntFromQText(ui->lineEdit_id_to->text(),10);
}
