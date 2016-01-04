#include "propertyfieldloaddialog.h"
#include "ui_propertyfieldloaddialog.h"
#include "Common/Constants.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <memory>
#include <QListWidgetItem>
PropertyFieldLoadDialog::PropertyFieldLoadDialog(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint),
	ui(new Ui::PropertyFieldLoadDialog)
{
	ui->setupUi(this);
	connect(ui->pushButton_load,SIGNAL(clicked(bool)), this, SLOT(onLoadClick(bool)) );
	connect(ui->pushButton_close,SIGNAL(clicked(bool)), this, SLOT(onCloseClick(bool)) );
}

PropertyFieldLoadDialog::~PropertyFieldLoadDialog()
{
	delete ui;
}

void PropertyFieldLoadDialog::onLoadClick(bool) {
	ui->pushButton_close->setVisible(true);
	ui->pushButton_load->setVisible(false);
	std::vector<std::shared_ptr<PropertyFieldDef>> selectedList;
	for(int i = 0; i < ui->listWidget_select_property_field->count(); i++) {
		QListWidgetItem* item = ui->listWidget_select_property_field->item(i);
		if(item->checkState() == Qt::Checked) {
			selectedList.push_back(propsList.at(i));
		}
	}
	onReadyToLoad(filename, selectedList);
}

void PropertyFieldLoadDialog::onCloseClick(bool) {
	//Dejar de cargar
	this->hide();
}

void PropertyFieldLoadDialog::setupForPropertyFields(std::string filename, std::vector<std::shared_ptr<PropertyFieldDef>> propertyFieldDefs){
	this->filename = filename;
	std::string bareFilename = FileUtils::getFileNameWithoutPath(filename);
	ui->label_property_field_name->setText(QString::fromStdString(bareFilename));
	propsList.clear();
	propsList.insert(propsList.begin(), propertyFieldDefs.begin(), propertyFieldDefs.end());
	ui->pushButton_close->setVisible(false);
	ui->pushButton_load->setVisible(true);
	ui->progressBar->setValue(0);
	ui->listWidget_select_property_field->clear();
	ui->listWidget_select_property_field->setEnabled(true);
	for(decltype(propertyFieldDefs.size()) i=0;i<propertyFieldDefs.size();i++) {
		QListWidgetItem* listItem = new QListWidgetItem(
					QString::fromStdString( propertyFieldDefs[i]->getName() ),
					ui->listWidget_select_property_field);
		listItem->setCheckState(Qt::Unchecked);
		ui->listWidget_select_property_field->addItem(listItem);

	}
	ui->plainTextEdit->clear();

	ui->progressBar->setMaximum(propertyFieldDefs[0]->getElementsCount());
	updateProgressBar();
}

void PropertyFieldLoadDialog::setPropertyFieldName(std::string propertyFieldName){
	ui->label_property_field_name->setText(QString::fromStdString(propertyFieldName));
}

void PropertyFieldLoadDialog::updateProgressBar(){
	ui->progressBar->setValue(progress);
}

//void PropertyFieldLoadDialog::stageComplete(PropertyFieldDef* propertyFieldDef){
	//QString message = QString::fromStdString(propertyFieldDef->getName() + " loaded...");
	//ui->plainTextEdit->appendPlainText(message);
	//progressStages++;
	//updateProgressBar();
//}

void PropertyFieldLoadDialog::setLoadedProgress(unsigned int progress){
	this->progress = progress;
	updateProgressBar();
}

void PropertyFieldLoadDialog::addMessage(QString message){
	ui->plainTextEdit->appendPlainText(message);
}

void PropertyFieldLoadDialog::displayWarning(QString message){
	ui->plainTextEdit->appendHtml(QString("<b style=\"color:orange;\">")+message+QString("</b>"));
}

void PropertyFieldLoadDialog::displayError(QString message){
	ui->plainTextEdit->appendHtml(QString("<b style=\"color:red;\">")+message+QString("</b>"));
	ui->progressBar->setEnabled(false);
}
