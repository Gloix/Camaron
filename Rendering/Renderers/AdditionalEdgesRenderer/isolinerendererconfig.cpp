#include "isolinerendererconfig.h"
#include "ui_isolinerendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>
#include "Rendering/RModel/rmodel.h"
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

void IsolineRendererConfig::setModel(RModel* model)
{
	if(this->model != model->getOriginalModel()) {
		this->model = model->getOriginalModel();
		scalarDefs.clear();
		scalarDefIdsMap.clear();
		scalarDefs = model->scalarDefs;
		ui->comboBox_prop_select->clear();
		if(scalarDefs.size() > 0) {
			ui->comboBox_prop_select->setEnabled(true);
			ui->horizontalSlider_sweep_value->setEnabled(true);
			for(std::vector<VScalarDef>::size_type i = 0;i<scalarDefs.size();i++){
				scalarDefIdsMap.insert(std::make_pair(i,scalarDefs[i]));
				ui->comboBox_prop_select->addItem(QString::fromUtf8(scalarDefs[i]->name), QVariant(i));
			}
			selectedScalarDef = scalarDefs[0];
		} else {
			ui->comboBox_prop_select->setEnabled(false);
			ui->horizontalSlider_sweep_value->setEnabled(false);
			ui->label_sweep_value->setText(QString(""));
			selectedScalarDef = NULL;
		}

	}
}

void IsolineRendererConfig::changeScalarPropFunc(int index){
	int propertyIndex = this->ui->comboBox_prop_select->itemData(index).toInt();
	selectedScalarDef = scalarDefs[propertyIndex];
}

void IsolineRendererConfig::changeInputType(int tabIndex) {
	this->currentInputType = tabIndex;
}

void IsolineRendererConfig::sweepValueChanged(int value) {
	isolinesSteps.clear();
	std::vector<float> scalarBounds = selectedScalarDef->bounds;
	int maxSliderValue = ui->horizontalSlider_sweep_value->maximum();
	int minSliderValue = ui->horizontalSlider_sweep_value->minimum();
	float isolineValue = scalarBounds[0] + 1.0*value*(scalarBounds[1]-scalarBounds[0])/(maxSliderValue-minSliderValue);
	ui->label_sweep_value->setText(QString::number(isolineValue));
	isolinesSteps.push_back(-1);
	isolinesSteps.push_back(isolineValue);
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
		isolinesSteps.clear();
		QString stepsStr = ui->lineEdit_steps->displayText();
		QStringList parts = stepsStr.split(QString(","), QString::SkipEmptyParts);
		foreach (const QString part, parts ) {
			isolinesSteps.push_back(atof(part.toStdString().c_str()));
		}
	}


	if(ui->radioButton_wf_no->isChecked())
		wireFrameOption = NO_WIREFRAME;
	if(ui->radioButton_wf_complete->isChecked())
		wireFrameOption = COMPLETE_WIREFRAME;
	if(ui->radioButton_wf_surface->isChecked())
		wireFrameOption = SURFACE_WIREFRAME;

}
