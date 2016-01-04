#include "isolinerendererconfig.h"
#include "ui_isolinerendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>
#include "Rendering/RModel/rmodel.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"

IsolineRendererConfig::IsolineRendererConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	ui(new Ui::IsolineRendererConfig)
{
	ui->setupUi(this);
	connect(ui->comboBox_prop_select, SIGNAL( currentIndexChanged(int)), this, SLOT(changeScalarPropFunc(int)));
	connect(ui->tabWidget_input_type, SIGNAL( currentChanged(int)), this, SLOT(changeInputType(int)));
	connect(ui->horizontalSlider_sweep_value, SIGNAL( valueChanged(int)), this, SLOT(sweepValueChanged(int)));
	readConfig();
}

IsolineRendererConfig::~IsolineRendererConfig()
{
	delete ui;
}

void IsolineRendererConfig::setRModel(RModel* rmodel) {
	this->rmodel = rmodel;
	if((Model*)this->model != rmodel->getOriginalModel()) {
		rmodel->getOriginalModel()->accept((ModelVisitor*)this);
	}
}

void IsolineRendererConfig::visit(PolygonMesh* model) {
	this->model = model;
	loadScalarDefs();
	scalarDefIdsMap.clear();
	ui->comboBox_prop_select->clear();
	if(scalarDefs.size() > 0) {
		ui->comboBox_prop_select->setEnabled(true);
		ui->horizontalSlider_sweep_value->setEnabled(true);
		for(auto i = 0u;i<scalarDefs.size();i++){
			scalarDefIdsMap.insert(std::make_pair(i,scalarDefs[i]));
			ui->comboBox_prop_select->addItem(QString::fromStdString(scalarDefs[i]->getName()), QVariant(i));
		}
		selectedScalarRModelDef = rmodel->loadPropertyField(model, std::dynamic_pointer_cast<ScalarFieldDef>(scalarDefs[0]));
	} else {
		ui->comboBox_prop_select->setEnabled(false);
		ui->horizontalSlider_sweep_value->setEnabled(false);
		ui->label_sweep_value->setText(QString(""));
		selectedScalarRModelDef = nullptr;
	}
}

//bool IsolineRendererConfig::setModel(Model* model) {
//	scalarDefs.clear();
//	scalarDefIdsMap.clear();
//	ui->comboBox_prop_select->clear();
//}

//void IsolineRendererConfig::loadScalarDefs() {
	//scalarDefs.clear();
	//this->model->addPropertyDefsTo((PropertyFieldDefList<PropertyFieldDef>&)scalarDefs);
//

void IsolineRendererConfig::loadScalarDefs() {
	scalarDefs.clear();
	ScalarFieldListAdderVisitor visitor(scalarDefs);
	for(std::shared_ptr<PropertyFieldDef> prop : this->model->getPropertyFieldDefs()) {
		prop->accept(visitor);
	}
}

void IsolineRendererConfig::changeScalarPropFunc(int index){
	int propertyIndex = this->ui->comboBox_prop_select->itemData(index).toInt();
	std::shared_ptr<ScalarFieldDef> selectedScalarDef = std::dynamic_pointer_cast<ScalarFieldDef>(scalarDefs[propertyIndex]);
	selectedScalarRModelDef = std::dynamic_pointer_cast<RModelPropertyFieldDef<ScalarFieldDef>>(rmodel->loadPropertyField(model, selectedScalarDef));
}

void IsolineRendererConfig::changeInputType(int tabIndex) {
	this->currentInputType = tabIndex;
}

void IsolineRendererConfig::sweepValueChanged(int value) {
	isolevels.clear();
	int maxSliderValue = ui->horizontalSlider_sweep_value->maximum();
	int minSliderValue = ui->horizontalSlider_sweep_value->minimum();
	std::shared_ptr<RModelPropertyFieldDef<ScalarFieldDef>> selected = std::dynamic_pointer_cast<RModelPropertyFieldDef<ScalarFieldDef>>(selectedScalarRModelDef->getPropertyFieldDef());
	float minScalar = selected->getPropertyFieldDef()->getMin();
	float maxScalar = selected->getPropertyFieldDef()->getMax();
	float isolineValue = minScalar + 1.0*value*(maxScalar-minScalar)/(maxSliderValue-minSliderValue);
	ui->label_sweep_value->setText(QString::number(isolineValue));
	isolevels.push_back(-1);
	isolevels.push_back(isolineValue);
	forceUpdate();
}

void IsolineRendererConfig::readConfig(){

	gradientStartColor.x = QtUtils::readFloatFromQText(ui->lineEdit_gs_r->text(),0.0f);
	gradientStartColor.y = QtUtils::readFloatFromQText(ui->lineEdit_gs_g->text(),0.0f);
	gradientStartColor.z = QtUtils::readFloatFromQText(ui->lineEdit_gs_b->text(),1.0f);
	gradientStartColor.w = QtUtils::readFloatFromQText(ui->lineEdit_gs_a->text(),1.0f);
	gradientEndColor.x = QtUtils::readFloatFromQText(ui->lineEdit_ge_r->text(),0.0f);
	gradientEndColor.y = QtUtils::readFloatFromQText(ui->lineEdit_ge_g->text(),1.0f);
	gradientEndColor.z = QtUtils::readFloatFromQText(ui->lineEdit_ge_b->text(),0.0f);
	gradientEndColor.w = QtUtils::readFloatFromQText(ui->lineEdit_ge_a->text(),1.0f);
	wireframeColor.x = QtUtils::readFloatFromQText(ui->lineEdit_wf_r->text(),1.0f);
	wireframeColor.y = QtUtils::readFloatFromQText(ui->lineEdit_wf_g->text(),1.0f);
	wireframeColor.z = QtUtils::readFloatFromQText(ui->lineEdit_wf_b->text(),0.0f);
	wireframeColor.w = QtUtils::readFloatFromQText(ui->lineEdit_wf_a->text(),1.0f);

	if(this->currentInputType == INPUT_VALUES_LIST) {
		isolevels.clear();
		QString stepsStr = ui->lineEdit_steps->displayText();
		QStringList parts = stepsStr.split(QString(","), QString::SkipEmptyParts);
		foreach (const QString part, parts ) {
			isolevels.push_back(atof(part.toStdString().c_str()));
		}
	}


	if(ui->radioButton_wf_no->isChecked())
		wireFrameOption = NO_WIREFRAME;
	if(ui->radioButton_wf_complete->isChecked())
		wireFrameOption = COMPLETE_WIREFRAME;
	if(ui->radioButton_wf_surface->isChecked())
		wireFrameOption = SURFACE_WIREFRAME;

}
