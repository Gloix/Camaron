#include "propertyfieldloaddialog.h"
#include "ui_propertyfieldloaddialog.h"
#include "Common/Constants.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include <iostream>
#include <memory>
#include <QListWidgetItem>
PropertyFieldLoadDialog::PropertyFieldLoadDialog(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint),
	ui(new Ui::PropertyFieldLoadDialog)
{
	ui->setupUi(this);
}

PropertyFieldLoadDialog::~PropertyFieldLoadDialog()
{
	delete ui;
}

void PropertyFieldLoadDialog::setupForPropertyFields(std::vector<std::shared_ptr<PropertyFieldDef>> propertyFieldDefs){
	ui->pushButton_load_close->setText("Load");
	ui->progressBar->setValue(0);
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
	ui->progressBar->setValue(progressStages);
}

void PropertyFieldLoadDialog::stageComplete(PropertyFieldDef* propertyFieldDef){
	QString message = QString::fromStdString(propertyFieldDef->getName() + " loaded...");
	ui->plainTextEdit->appendPlainText(message);
	progressStages++;
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
