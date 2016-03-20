#include "rendererconfigpopup.h"
#include "ui_rendererconfigpopup.h"
#include "Utils/crosstimer.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Common/Constants.h"

RendererConfigPopUp::RendererConfigPopUp(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint ),
	ui(new Ui::RendererConfigPopUp)
{
	ui->setupUi(this);
	connect(this->ui->pushButton_apply_changes, SIGNAL(clicked()), this, SLOT(applyChangesPushButtonClicked()));
}

RendererConfigPopUp::~RendererConfigPopUp()
{
	delete ui;
}
void RendererConfigPopUp::setConfigQWidget(BaseRendererConfig* qwidget){
	this->cleanConfigQWidget();
	connect(qwidget, SIGNAL(onConfigForceApply()), this, SLOT(applyChangesPushButtonClicked()));
	ui->widget_renderer_config_container->layout()->addWidget((QWidget*) qwidget);
	((QWidget*)qwidget)->show();
	this->ui->pushButton_apply_changes->setDisabled(false);
}

void RendererConfigPopUp::cleanConfigQWidget(){
	this->ui->pushButton_apply_changes->setDisabled(true);
	if(!this->ui->widget_renderer_config_container->layout()->isEmpty()){
		QLayoutItem* item;
		while ( (item = ui->widget_renderer_config_container->layout()->takeAt(0)) != 0){
			QWidget * widget = item->widget();
			if(widget!=0)
				widget->hide();
		}
	}
}
void RendererConfigPopUp::applyChangesPushButtonClicked(){
	#ifdef DEBUG_MOD
	glFinish();
	CrossTimer timer;
	emit applyChangesPushButton();
	glFinish();
	long nanos = timer.getTranscurredNanoseconds();
	std::cout << "UPDATE: " << nanos << std::endl;
	#else
	emit applyChangesPushButton();
	#endif
}
