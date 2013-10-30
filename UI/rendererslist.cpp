#include "rendererslist.h"
#include "ui_rendererslist.h"
#include "Rendering/Renderer.h"
#include <QKeyEvent>
#include <iostream>
RenderersList::RenderersList(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RenderersList)
{
	ui->setupUi(this);
//	ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
}

RenderersList::~RenderersList()
{
	delete ui;
}

void RenderersList::keyPressEvent(QKeyEvent* event){
	if(event->key()==Qt::Key_Delete){
		if(!renderers.size())
			return;
		int itemIndex = ui->listWidget->currentIndex().row();
		QListWidgetItem* item = ui->listWidget->takeItem(itemIndex);
		delete item;
		removeRenderer((unsigned int)itemIndex);
	}
}
std::vector<Renderer*>& RenderersList::getRenderers(){
	return renderers;
}

void RenderersList::addRenderer(Renderer* r){
	if(!r)
		return;
	for(unsigned int i = 0;i<renderers.size();i++)
		if(renderers[i]==r)
			return;
	renderers.push_back(r);
	ui->listWidget->addItem(new QListWidgetItem(QString(r->getRendererName())));
}
void RenderersList::removeRenderer(unsigned int pos){
	if(pos==renderers.size())//not found
		return;
	for(unsigned int i = pos;i<renderers.size()-1;i++)
		renderers[i] = renderers[i+1];
	renderers.pop_back();
	emit secondaryRendererRemoved();
}

void RenderersList::removeAllRenderers(){
	renderers.clear();
	ui->listWidget->clear();
}
