#include "aboutcamaron.h"
#include "ui_aboutcamaron.h"
AboutCamaron::AboutCamaron(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint),
	ui(new Ui::AboutCamaron)
{
	ui->setupUi(this);
}

AboutCamaron::~AboutCamaron()
{
	delete ui;
}
