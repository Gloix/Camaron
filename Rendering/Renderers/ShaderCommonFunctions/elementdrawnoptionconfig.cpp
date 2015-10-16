#include "elementdrawnoptionconfig.h"
#include "ui_elementdrawnoptionconfig.h"

ElementDrawnOptionConfig::ElementDrawnOptionConfig(QWidget *parent) :
    BaseRendererConfig(parent),
	ui(new Ui::ElementDrawnOptionConfig)
{
	ui->setupUi(this);
}

ElementDrawnOptionConfig::~ElementDrawnOptionConfig()
{
	delete ui;
}

void ElementDrawnOptionConfig::readConfig(){
	elementDrawnOption = DRAW_ALL;
	if(ui->radioButton_selected->isChecked())
		elementDrawnOption = DRAW_ONLY_SELECTED;
	else if(ui->radioButton_unselected->isChecked())
		elementDrawnOption = DRAW_ONLY_UNSELECTED;
}
