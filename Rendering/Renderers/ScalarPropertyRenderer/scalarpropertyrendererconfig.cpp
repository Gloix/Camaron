#include "scalarpropertyrendererconfig.h"
#include "ui_scalarpropertyrendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>
#include "Rendering/RModel/rmodel.h"

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
	selectedScalarDef = NULL;

}

void ScalarPropertyRendererConfig::loadBoundsFromModel(){
    setBoundsFromModel();
}

void ScalarPropertyRendererConfig::changedProperty(int index){
	selectedScalarDef = scalarDefIdsMap[index];
	std::vector<float> bounds = selectedBounds[selectedScalarDef];
	if(bounds.size() == 2) {
		ui->lineEdit_min_bound->setText(QString::number(bounds[0]));
		ui->lineEdit_max_bound->setText(QString::number(bounds[1]));
	}
}

ScalarPropertyRendererConfig::~ScalarPropertyRendererConfig()
{
	delete ui;
}

void ScalarPropertyRendererConfig::setModel(RModel* model)
{
	if(this->model != model->getOriginalModel()) {
		this->model = model->getOriginalModel();
		scalarDefs.clear();
		scalarDefIdsMap.clear();
		scalarDefs = model->scalarDefs;
		selectedBounds.clear();
		ui->comboBox_prop_select->clear();
		if(scalarDefs.size() > 0) {
			ui->comboBox_prop_select->setEnabled(true);
			for(std::vector<VScalarDef>::size_type i = 0;i<scalarDefs.size();i++){
				selectedBounds[scalarDefs[i]] = scalarDefs[i]->bounds;
				scalarDefIdsMap.insert(std::make_pair(i,scalarDefs[i]));
				ui->comboBox_prop_select->addItem(QString::fromStdString(scalarDefs[i]->name), QVariant(i));
			}
		} else {
			ui->comboBox_prop_select->setEnabled(false);
			selectedScalarDef = NULL;
		}

	}
}

bool ScalarPropertyRendererConfig::setBoundsFromModel(){
    if (rmodel==NULL || rmodel->bounds.size()!=6) {
        std::cout << "BOUNDS NOT AVAILABLE." << std::endl;
        return false;
    }
	selectedBounds[selectedScalarDef] = selectedScalarDef->bounds;
    //std::cout << "BOUNDS AVAILABLE:" <<min_x<<","<<min_y<<","<<min_z<< std::endl;
    //std::cout << "BOUNDS AVAILABLE:" <<max_x<<","<<max_y<<","<<max_z<< std::endl;
    //update_ui
	//int precision = 8;
	ui->lineEdit_min_bound->setText(QString::number(selectedBounds[selectedScalarDef][0]));
	ui->lineEdit_max_bound->setText(QString::number(selectedBounds[selectedScalarDef][1]));

	ui->lineEdit_min_bound->setCursorPosition(0);
	ui->lineEdit_max_bound->setCursorPosition(0);
    return true;
}

void ScalarPropertyRendererConfig::readConfig(){

    if (ui->radioButton_Color_mode_hue->isChecked()) coloring_type = 1;
    else coloring_type = 0; //gray scale

    if (ui->checkBox_Inverse_intensity->isChecked()) inverse_intensity = 1;
    else inverse_intensity = 0;

	if(selectedScalarDef != NULL) {
		selectedBounds[selectedScalarDef][0] =
				QtUtils::readFloatFromQText(ui->lineEdit_min_bound->text(),selectedScalarDef->bounds[0]);
		selectedBounds[selectedScalarDef][1] =
				QtUtils::readFloatFromQText(ui->lineEdit_max_bound->text(),selectedScalarDef->bounds[1]);
	}

}
