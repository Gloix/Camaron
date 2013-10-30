#include "heightrendererconfig.h"
#include "ui_heightrendererconfig.h"
#include "Utils/qtutils.h"
#include <iostream>
#include "Rendering/RModel/rmodel.h"

HeightRendererConfig::HeightRendererConfig(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::HeightRendererConfig)
{
	ui->setupUi(this);
    connect(ui->pushButton,SIGNAL( clicked()),this,SLOT(loadBoundsFromModel()));
    rmodel = NULL;
    axis = 2; //z axis
    coloring_type = 1;
    inverse_intensity = 0;
	min_x = 0.0f;
    min_y = 0.0f;
    min_z = 0.0f;
    max_x = 1.0f;
    max_y = 1.0f;
	max_z = 1.0f;
}

void HeightRendererConfig::loadBoundsFromModel(){
    setBoundsFromModel();
}

HeightRendererConfig::~HeightRendererConfig()
{
	delete ui;
}

void HeightRendererConfig::setModel(RModel* model){
    rmodel=model;
}

bool HeightRendererConfig::setBoundsFromModel(){
    if (rmodel==NULL || rmodel->bounds.size()!=6) {
        std::cout << "BOUNDS NOT AVAILABLE." << std::endl;
        return false;
    }
    min_x = rmodel->bounds[0];
    min_y = rmodel->bounds[1];
    min_z = rmodel->bounds[2];
    max_x = rmodel->bounds[3];
    max_y = rmodel->bounds[4];
    max_z = rmodel->bounds[5];
    //std::cout << "BOUNDS AVAILABLE:" <<min_x<<","<<min_y<<","<<min_z<< std::endl;
    //std::cout << "BOUNDS AVAILABLE:" <<max_x<<","<<max_y<<","<<max_z<< std::endl;
    //update_ui
    int precision = 8;
    ui->lineEdit_min_x->setText(QString::number(min_x,'f',precision));
    ui->lineEdit_min_y->setText(QString::number(min_y,'f',precision));
    ui->lineEdit_min_z->setText(QString::number(min_z,'f',precision));
    ui->lineEdit_maz_x->setText(QString::number(max_x,'f',precision));
    ui->lineEdit_max_y->setText(QString::number(max_y,'f',precision));
    ui->lineEdit_max_z->setText(QString::number(max_z,'f',precision));

    ui->lineEdit_min_x->setCursorPosition(0);
    ui->lineEdit_min_y->setCursorPosition(0);
    ui->lineEdit_min_z->setCursorPosition(0);
    ui->lineEdit_maz_x->setCursorPosition(0);
    ui->lineEdit_max_y->setCursorPosition(0);
    ui->lineEdit_max_z->setCursorPosition(0);
    return true;
}

void HeightRendererConfig::readConfig(){
    if (ui->radioButton_Axis_X->isChecked()) axis = 0; //X axis
    else if(ui->radioButton_Axis_Y->isChecked()) axis = 1; //Y axis
    else axis = 2; //z axis

    if (ui->radioButton_Color_mode_hue->isChecked()) coloring_type = 1;
    else coloring_type = 0; //gray scale

    if (ui->checkBox_Inverse_intensity->isChecked()) inverse_intensity = 1;
    else inverse_intensity = 0;

    min_x = QtUtils::readFloatFromQText(ui->lineEdit_min_x->text(),min_x);
    min_y = QtUtils::readFloatFromQText(ui->lineEdit_min_y->text(),min_y);
    min_z = QtUtils::readFloatFromQText(ui->lineEdit_min_z->text(),min_z);
    max_x = QtUtils::readFloatFromQText(ui->lineEdit_maz_x->text(),max_x);
    max_y = QtUtils::readFloatFromQText(ui->lineEdit_max_y->text(),max_y);
    max_z = QtUtils::readFloatFromQText(ui->lineEdit_max_z->text(),max_z);

}
