#include "modelloadingprogressdialog.h"
#include "ui_modelloadingprogressdialog.h"
#include "Common/Constants.h"
#include <iostream>
ModelLoadingProgressDialog::ModelLoadingProgressDialog(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint),
	ui(new Ui::ModelLoadingProgressDialog)
{
	ui->setupUi(this);
}

ModelLoadingProgressDialog::~ModelLoadingProgressDialog()
{
	delete ui;
}
void ModelLoadingProgressDialog::setupForNewModel(int type,
												  int nvertices,
												  int npolygons,
												  int npolyhedrons){
	modelType = type;
	ui->progressBar->setEnabled(true);
	ui->pushButton->setVisible(false);
	resetVertices(nvertices);
	resetPolygons(npolygons);
	resetPolyhedrons(npolyhedrons);
	ui->progressBar->setValue(0);
	progressStages = 0;
	ui->plainTextEdit->clear();
	totalelements = totalVertices+totalPolygons+totalPolyhedrons;
	switch(type){
		case vis::CONSTANTS::VERTEX_CLOUD:{
			ui->widget_polygon->setVisible(false);
			ui->widget_polyhedron->setVisible(false);
			progressVarMax = (int)((nvertices+npolygons+npolyhedrons)*1.3f);
			break;
		}
		case vis::CONSTANTS::POLYGON_MESH:{
			ui->widget_polygon->setVisible(true);
			ui->widget_polyhedron->setVisible(false);
			progressVarMax = (int)((nvertices+npolygons+npolyhedrons)*1.6f);
			break;
		}
		case vis::CONSTANTS::POLYHEDRON_MESH:{
			ui->widget_polygon->setVisible(true);
			ui->widget_polyhedron->setVisible(true);
			progressVarMax = (int)((nvertices+npolygons+npolyhedrons)*1.9f);
			break;
		}
		case vis::CONSTANTS::LIGHT_WEIGHT_VERTEX_CLOUD:{
			ui->widget_polygon->setVisible(false);
			ui->widget_polyhedron->setVisible(false);
			progressVarMax = (int)((nvertices+npolygons+npolyhedrons)*1.3f);
			break;
		}
		case vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH:{
			ui->widget_polygon->setVisible(true);
			ui->widget_polyhedron->setVisible(false);
			progressVarMax = (int)((nvertices+npolygons+npolyhedrons)*1.4f);
			break;
		}
		case vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH:{
			ui->widget_polygon->setVisible(true);
			ui->widget_polyhedron->setVisible(true);
			progressVarMax = (int)((nvertices+npolygons+npolyhedrons)*1.5f);
			break;
		}
		default:
			std::cerr << "Error [ModelLoadingProgressDialog]:"<<
						 "Invalid model type: "<<type<<std::endl;
	}
	ui->progressBar->setMaximum(progressVarMax);
	updateProgressBar();
}
void ModelLoadingProgressDialog::setModelName(std::string modelname){
	ui->label_model_name->setText(QString::fromStdString(modelname));
}

void ModelLoadingProgressDialog::resetVertices(int n){
	loadedVertices = 0;
	totalVertices = n;
	setVerticesLabels();
}

void ModelLoadingProgressDialog::resetPolygons(int n){
	loadedPolygons = 0;
	totalPolygons = n;
	setPolygonsLabels();
}

void ModelLoadingProgressDialog::resetPolyhedrons(int n){
	loadedPolyhedrons = 0;
	totalPolyhedrons = n;
	setPolyhedronsLabels();
}
void ModelLoadingProgressDialog::setVerticesLabels(){
	ui->label_loaded_vertices->setText(QString::number(loadedVertices));
	ui->label_vertices_percent->setText(QString::number(((float)loadedVertices)/
														((float)totalVertices)*
														100.0f)+QString("%"));
	ui->label_vertices_to_load->setText(QString::number(totalVertices));
}

void ModelLoadingProgressDialog::setPolygonsLabels(){
	ui->label_loaded_polygons->setText(QString::number(loadedPolygons));
	ui->label_polygons_percent->setText(QString::number(((float)loadedPolygons)/
														((float)totalPolygons)*
														100.0f)+QString("%"));
	ui->label_polygons_to_load->setText(QString::number(totalPolygons));
}

void ModelLoadingProgressDialog::setPolyhedronsLabels(){
	ui->label_loaded_polyhedrons->setText(QString::number(loadedPolyhedrons));
	ui->label_polyhedrons_percent->setText(QString::number(((float)loadedPolyhedrons)/
														   ((float)totalPolyhedrons)*
														   100.0f)+QString("%"));
	ui->label_polyhedrons_to_load->setText(QString::number(totalPolyhedrons));
}

void ModelLoadingProgressDialog::setLoadedVertices(int n){
	if(loadedVertices!=n){
		loadedVertices = n;
		setVerticesLabels();
		updateProgressBar();
	}
}

void ModelLoadingProgressDialog::setLoadedPolygons(int n){
	if(loadedPolygons!=n){
		loadedPolygons = n;
		setPolygonsLabels();
		updateProgressBar();
	}
}

void ModelLoadingProgressDialog::setLoadedPolyhedrons(int n){
	if(loadedPolyhedrons!=n){
		loadedPolyhedrons = n;
		setPolyhedronsLabels();
		updateProgressBar();
	}
}
void ModelLoadingProgressDialog::updateProgressBar(){
	progressVarValue = loadedVertices+loadedPolygons+
					   loadedPolyhedrons + progressStages;
	ui->progressBar->setValue(progressVarValue);
}
void ModelLoadingProgressDialog::setupProgressBarForNewModel(int mtype,int v,int p,int ph){
	setupForNewModel(mtype,v,p,ph);
}
void ModelLoadingProgressDialog::stageComplete(int d){
	QString message("");
	switch(d){
		case RMODEL_READY:{
			message = QString("Rendering model complete...");break;
		}
		case CAMARON_ELEMENTS_CONFIGURATED:{
			message = QString("Camaron elements configurated...");break;
		}
		case COMPLETED_VERTEX_POLYGON_R:{
			message = QString("Completed vertex-polygon neighboor pointers...");break;
		}
		case COMPLETED_POLYGON_POLYGON_R:{
			message = QString("Completed polygon-polygon neighboor pointers...");break;
		}
		case NORMALS_CALCULATED:{
			message = QString("Normals calculated...");break;
		}
		case COMPLETED_POLYGON_POLYHEDRON_R:{
			message = QString("Completed polygons-polyhedron neighboor pointers...");break;
		}
		case POLYHEDRON_GEOCENTER_CALCULATED:{
			message = QString("Polyhedron's geocenters calculated...");break;
		}
		case FIXED_SURFACE_POLYGONS_VERTICES_ORDER:{
			message = QString("Fixed surface polygons vertices order...");break;
		}
		case BASE_MODE_READY:{
			message = QString("Base model loaded...");break;
		}
	}
	ui->plainTextEdit->appendPlainText(message);
	progressStages+=0.1f*(totalVertices+totalPolygons+totalPolyhedrons);
	updateProgressBar();
}

void ModelLoadingProgressDialog::addMessage(QString message){
	ui->plainTextEdit->appendPlainText(message);
}
void ModelLoadingProgressDialog::displayWarning(QString message){
	ui->plainTextEdit->appendHtml(QString("<b style=\"color:orange;\">")+message+QString("</b>"));
}
void ModelLoadingProgressDialog::displayError(QString message){
	ui->plainTextEdit->appendHtml(QString("<b style=\"color:red;\">")+message+QString("</b>"));
	ui->progressBar->setEnabled(false);
	ui->pushButton->setVisible(true);
}
