#include "isolinerendererconfig.h"
#include "ui_isolinerendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>
#include "Rendering/RModel/rmodel.h"
IsolineRendererConfig::IsolineRendererConfig(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::IsolineRendererConfig)
{
	ui->setupUi(this);
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
        scalarDefs = model->scalarDefs;
        if(scalarDefs.size() > 0) {
            connect(ui->comboBox_prop_select,SIGNAL( currentIndexChanged(int)),this, SLOT(changeScalarPropFunc(int)));
            for(std::vector<VScalarDef>::size_type i = 0;i<scalarDefs.size();i++){
                scalarDefIdsMap.insert(std::make_pair(i,scalarDefs[i]));
                ui->comboBox_prop_select->addItem(QString::fromUtf8(scalarDefs[i]->name), QVariant(i));
            }
            selectedScalarDef = scalarDefs[0];
        }

    }
}

void IsolineRendererConfig::changeScalarPropFunc(int index){
    int key = this->ui->comboBox_prop_select->itemData(index).toInt();
    selectedScalarDef = scalarDefs[key];
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

    isolinesSteps.clear();
    QString stepsStr = ui->lineEdit_steps->displayText();
    QStringList parts = stepsStr.split(QString(","), QString::SkipEmptyParts);
    foreach (const QString part, parts ) {
        isolinesSteps.push_back(atof(part.toStdString().c_str()));
    }


	if(ui->radioButton_wf_no->isChecked())
		wireFrameOption = NO_WIREFRAME;
	if(ui->radioButton_wf_complete->isChecked())
		wireFrameOption = COMPLETE_WIREFRAME;
	if(ui->radioButton_wf_surface->isChecked())
		wireFrameOption = SURFACE_WIREFRAME;

}
