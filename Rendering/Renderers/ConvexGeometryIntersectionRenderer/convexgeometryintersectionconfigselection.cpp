#include "convexgeometryintersectionconfigselection.h"
#include "ui_convexgeometryintersectionconfigselection.h"
#include "Common/Constants.h"
ConvexGeometryIntersectionConfigSelection::ConvexGeometryIntersectionConfigSelection(QWidget *parent) :
	BaseRendererConfig(parent),
	ui(new Ui::ConvexGeometryIntersectionConfigSelection)
{
	ui->setupUi(this);
	readConfig();
}

ConvexGeometryIntersectionConfigSelection::~ConvexGeometryIntersectionConfigSelection()
{
	delete ui;
}
void ConvexGeometryIntersectionConfigSelection::readConfig(){
	if(ui->radio_polygon->isChecked())
		elementsType = vis::CONSTANTS::POLYGON;
	if(ui->radio_polyhedron->isChecked())
		elementsType = vis::CONSTANTS::POLYHEDRON;
	if(ui->radio_new_selection->isChecked())
		selectionType = ConvexGeometryIntersectionConfigSelection::NEW_SELECTION;
	if(ui->radio_join_selection->isChecked())
		selectionType = ConvexGeometryIntersectionConfigSelection::JOIN_SELECTION;
	if(ui->radio_intersect_selection->isChecked())
		selectionType = ConvexGeometryIntersectionConfigSelection::INTERSECT_SELECTION;
	if(ui->radio_invert->isChecked())
		selectionType = ConvexGeometryIntersectionConfigSelection::INVERT_SELECTION;

}
