#include "scalarpropertyrendererconfig.h"
#include "ui_scalarpropertyrendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>
#include "Rendering/RModel/rmodel.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"

ScalarPropertyRendererConfig::ScalarPropertyRendererConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	ui(new Ui::ScalarPropertyRendererConfig)
{
	ui->setupUi(this);
	connect(ui->pushButton,SIGNAL( clicked()),this,SLOT(loadBoundsFromModel()));
	connect(ui->comboBox_prop_select,SIGNAL( currentIndexChanged(int)),this,SLOT(changedProperty(int)));
	rmodel = NULL;
	coloring_type = 1;
	inverse_intensity = 0;
}

void ScalarPropertyRendererConfig::loadBoundsFromModel(){
	setBoundsFromModel();
}

void ScalarPropertyRendererConfig::changedProperty(int index){
	if(index < 0) return;
	selectedScalarDef = scalarFields[index];
	std::vector<float> bounds = selectedBounds[(PropertyFieldDef*)selectedScalarDef.get()];
	if(bounds.size() == 2) {
		ui->lineEdit_min_bound->setText(QString::number(bounds[0]));
		ui->lineEdit_max_bound->setText(QString::number(bounds[1]));
	}
}

ScalarPropertyRendererConfig::~ScalarPropertyRendererConfig()
{
	delete ui;
}

void ScalarPropertyRendererConfig::setScalarFields(std::vector<std::shared_ptr<ScalarFieldDef>> &scalarFields)
{
	this->scalarFields = scalarFields;
	//scalarDefIdsMap.clear();

	ui->comboBox_prop_select->clear();
	selectedBounds.clear();
	if(scalarFields.size() > 0) {
		ui->comboBox_prop_select->setEnabled(true);
		for(decltype(scalarFields.size()) i = 0;i<scalarFields.size();i++){
			float max = scalarFields[i]->getMax();
			float min = scalarFields[i]->getMin();
			selectedBounds[(PropertyFieldDef*)scalarFields[i].get()] = std::vector<float>(2);
			selectedBounds[(PropertyFieldDef*)scalarFields[i].get()][0] = min;
			selectedBounds[(PropertyFieldDef*)scalarFields[i].get()][1] = max;
			//scalarDefIdsMap.insert(std::make_pair(i,scalarFields[i]));
			ui->comboBox_prop_select->addItem(QString::fromStdString(scalarFields[i]->getName()), QVariant(i));
		}
		selectedScalarDef = scalarFields[0];
	} else {
		ui->comboBox_prop_select->setEnabled(false);
		selectedScalarDef = nullptr;
	}
}

//void ScalarPropertyRendererConfig::visit(PolygonMesh* model) {
//	this->model = model;
//	onNewModelLoaded();
//}

//void ScalarPropertyRendererConfig::visit(PolyhedronMesh* model) {
//	this->model = (Model*)model;
//	onNewModelLoaded();
//}

//void ScalarPropertyRendererConfig::onNewModelLoaded()
//{
//	loadScalarDefs();
//	scalarDefIdsMap.clear();
//	ui->comboBox_prop_select->clear();
//	selectedBounds.clear();
//	if(scalarFields.size() > 0) {
//		ui->comboBox_prop_select->setEnabled(true);
//		for(decltype(scalarFields.size()) i = 0;i<scalarFields.size();i++){
//			float max = scalarFields[i]->getMax();
//			float min = scalarFields[i]->getMin();
//			selectedBounds[(PropertyFieldDef*)scalarFields[i].get()] = std::vector<float>(2);
//			selectedBounds[(PropertyFieldDef*)scalarFields[i].get()][0] = min;
//			selectedBounds[(PropertyFieldDef*)scalarFields[i].get()][1] = max;
//			scalarDefIdsMap.insert(std::make_pair(i,scalarFields[i]));
//			ui->comboBox_prop_select->addItem(QString::fromStdString(scalarFields[i]->getName()), QVariant(i));
//		}
//		selectedScalarRModelDef = rmodel->loadPropertyField(std::dynamic_pointer_cast<ScalarFieldDef>(scalarFields[0]));
//	} else {
//		ui->comboBox_prop_select->setEnabled(false);
//		selectedScalarRModelDef = nullptr;
//	}
//}

//void ScalarPropertyRendererConfig::loadScalarDefs() {
//	scalarDefs.clear();
//	ScalarFieldListAdderVisitor visitor(scalarDefs);
//	for(std::shared_ptr<PropertyFieldDef> prop : this->model->getPropertyFieldDefs()) {
//		prop->accept(visitor);
//	}
//}

bool ScalarPropertyRendererConfig::setBoundsFromModel(){
	if (rmodel==NULL || rmodel->bounds.size()!=6) {
		std::cout << "BOUNDS NOT AVAILABLE." << std::endl;
		return false;
	}
	float max = selectedScalarDef->getMax();
	float min = selectedScalarDef->getMin();
	selectedBounds[(PropertyFieldDef*)selectedScalarDef.get()][0] = min;
	selectedBounds[(PropertyFieldDef*)selectedScalarDef.get()][1] = max;
	//std::cout << "BOUNDS AVAILABLE:" <<min_x<<","<<min_y<<","<<min_z<< std::endl;
	//std::cout << "BOUNDS AVAILABLE:" <<max_x<<","<<max_y<<","<<max_z<< std::endl;
	//update_ui
	//int precision = 8;
	ui->lineEdit_min_bound->setText(QString::number(min));
	ui->lineEdit_max_bound->setText(QString::number(max));

	ui->lineEdit_min_bound->setCursorPosition(0);
	ui->lineEdit_max_bound->setCursorPosition(0);
	return true;
}

void ScalarPropertyRendererConfig::readConfig(){

	if (ui->radioButton_Color_mode_hue->isChecked()) coloring_type = 1;
	else coloring_type = 0; //gray scale

	if (ui->checkBox_Inverse_intensity->isChecked()) inverse_intensity = 1;
	else inverse_intensity = 0;

	if(selectedScalarDef) {
		selectedBounds[(PropertyFieldDef*)selectedScalarDef.get()][0] =
				QtUtils::readFloatFromQText(ui->lineEdit_min_bound->text(),selectedScalarDef->getMin());
		selectedBounds[(PropertyFieldDef*)selectedScalarDef.get()][1] =
				QtUtils::readFloatFromQText(ui->lineEdit_max_bound->text(),selectedScalarDef->getMax());
	}

}
