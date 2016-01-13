#include "IsosurfaceRendererConfig.h"
#include "ui_isosurfacerendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>
#include "Rendering/RModel/rmodel.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "Model/PolyhedronMesh.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "PropertyFieldLoading/scalarfieldlistaddervisitor.h"

IsosurfaceRendererConfig::IsosurfaceRendererConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	ModelVisitor(),
	ui(new Ui::IsosurfaceRendererConfig)
{
	ui->setupUi(this);
	connect(ui->comboBox_prop_select, SIGNAL( currentIndexChanged(int)), this, SLOT(changeScalarPropFunc(int)));
	connect(ui->tabWidget_input_type, SIGNAL( currentChanged(int)), this, SLOT(changeInputType(int)));
	connect(ui->horizontalSlider_sweep_value, SIGNAL( valueChanged(int)), this, SLOT(sweepValueChanged(int)));
	readConfig();
}

IsosurfaceRendererConfig::~IsosurfaceRendererConfig()
{
	delete ui;
}

void IsosurfaceRendererConfig::setRModel(RModel* rmodel)
{
	this->rmodel = rmodel;
	if((Model*)this->model != rmodel->getOriginalModel()) {
		rmodel->getOriginalModel()->accept((ModelVisitor*)this);
	}
}

void IsosurfaceRendererConfig::visit(PolyhedronMesh* model) {
	this->model = model;
	loadScalarDefs();
	scalarDefIdsMap.clear();
	ui->comboBox_prop_select->clear();
	if(scalarDefs.size() > 0) {
		ui->comboBox_prop_select->setEnabled(true);
		ui->horizontalSlider_sweep_value->setEnabled(true);
		ui->horizontalSlider_sweep_value->setValue(ui->horizontalSlider_sweep_value->minimum());
		for(decltype(scalarDefs.size()) i = 0;i<scalarDefs.size();i++){
			scalarDefIdsMap.insert(std::make_pair(i,scalarDefs[i]));
			ui->comboBox_prop_select->addItem(QString::fromStdString(scalarDefs[i]->getName()), QVariant(i));
		}
		selectedScalarRModelDef = rmodel->loadPropertyField((VertexCloud*)model, std::dynamic_pointer_cast<ScalarFieldDef>(scalarDefs[0]));
		ui->label_sweep_value->setText(QString::number(selectedScalarRModelDef->getPropertyFieldDef()->getMin()));
	} else {
		ui->comboBox_prop_select->setEnabled(false);
		ui->horizontalSlider_sweep_value->setEnabled(false);
		ui->label_sweep_value->setText(QString(""));
		selectedScalarRModelDef = nullptr;
	}
}

void IsosurfaceRendererConfig::loadScalarDefs() {
	scalarDefs.clear();
	ScalarFieldListAdderVisitor visitor(scalarDefs);
	for(std::shared_ptr<PropertyFieldDef> prop : this->model->getPropertyFieldDefs()) {
		prop->accept(visitor);
	}
}



void IsosurfaceRendererConfig::changeScalarPropFunc(int index){
	int propertyIndex = this->ui->comboBox_prop_select->itemData(index).toInt();
	std::shared_ptr<ScalarFieldDef> selectedScalarDef = std::dynamic_pointer_cast<ScalarFieldDef>(scalarDefs[propertyIndex]);
	selectedScalarRModelDef = std::dynamic_pointer_cast<RModelPropertyFieldDef<ScalarFieldDef>>(rmodel->loadPropertyField(model, selectedScalarDef));
}

void IsosurfaceRendererConfig::changeInputType(int tabIndex) {
	this->currentInputType = tabIndex;
	if(tabIndex == INPUT_SWEEP) {
		sweepValueChanged(ui->horizontalSlider_sweep_value->value());
	} else {
		readInputIsolevels();
		forceUpdate();
	}
}

void IsosurfaceRendererConfig::sweepValueChanged(int value) {
	isolevels.clear();
	float maxScalar = selectedScalarRModelDef->getPropertyFieldDef()->getMax();
	float minScalar = selectedScalarRModelDef->getPropertyFieldDef()->getMin();
	int maxSliderValue = ui->horizontalSlider_sweep_value->maximum();
	int minSliderValue = ui->horizontalSlider_sweep_value->minimum();
	float isolevel = minScalar + 1.0*value*(maxScalar-minScalar)/(maxSliderValue-minSliderValue);
	ui->label_sweep_value->setText(QString::number(isolevel));
	isolevels.push_back(isolevel);
	forceUpdate();
}

void IsosurfaceRendererConfig::readConfig(){

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
		readInputIsolevels();
	}


	if(ui->radioButton_wf_no->isChecked())
		wireFrameOption = NO_WIREFRAME;
	if(ui->radioButton_wf_complete->isChecked())
		wireFrameOption = COMPLETE_WIREFRAME;
	if(ui->radioButton_wf_surface->isChecked())
		wireFrameOption = SURFACE_WIREFRAME;

}

IsosurfaceRendererConfig::readInputIsolevels() {
	isolevels.clear();
	QString stepsStr = ui->lineEdit_steps->displayText();
	QStringList parts = stepsStr.split(QString(","), QString::SkipEmptyParts);
	foreach (const QString part, parts ) {
		isolevels.push_back(atof(part.toStdString().c_str()));
	}
}
