#include "selectiontableview.h"
#include "ui_selectiontableview.h"
#include "Utils/crosstimer.h"
#include "Utils/qtutils.h"
#include "Utils/elementutils.h"
#include "Common/Constants.h"
#include <QStandardItemModel>
#include <iostream>
#include <algorithm>
#include "SelectionStrategies/Selection.h"
#include "Model/Element/Element.h"

#define ITEMS_PER_PAGE 1000

SelectionTableView::SelectionTableView(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint),
	ui(new Ui::SelectionTableView)
{
	ui->setupUi(this);
	ui->label_elements_type->setText(QString("None"));
	ui->label_number_ele->setText(QString("0"));
	ui->tableView_selected_elements->setContextMenuPolicy(Qt::ActionsContextMenu);
	QAction *action1 = new QAction(QString("Resize Rows to Contents"),ui->tableView_selected_elements);
	QAction *action2 = new QAction(QString("Resize Columns to Contents"),ui->tableView_selected_elements);
	QAction *action3 = new QAction(QString("Refresh Properties"),ui->tableView_selected_elements);
	ui->tableView_selected_elements->addAction(action1);
	ui->tableView_selected_elements->addAction(action2);
	ui->tableView_selected_elements->addAction(action3);
	connect(action1,SIGNAL(triggered()),ui->tableView_selected_elements,SLOT(resizeRowsToContents()));
	connect(action2,SIGNAL(triggered()),ui->tableView_selected_elements,SLOT(resizeColumnsToContents()));
	connect(action3,SIGNAL(triggered()),this,SLOT(refreshPage()));
	//ui->tableView_selected_elements->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	//ui->tableView_selected_elements->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	//ui->tableView_selected_elements->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	selection = (Selection*)0;
	ui->widget_selection_pages->layout()->setAlignment(ui->widget_selection_page_scroll,Qt::AlignHCenter);
	ui->widget_selection_pages->layout()->setAlignment(ui->widget_selection_number_of_pages,Qt::AlignHCenter);
	ui->widget_selection_pages->hide();
	currentPage = 1;
	maxPages = 1;
	connect(this->ui->pushButton_selection_page_back,SIGNAL(clicked()),this,SLOT(lastPage()));
	connect(this->ui->pushButton_selection_page_next,SIGNAL(clicked()),this,SLOT(nextPage()));
	connect(this->ui->lineEdit_selection_pages_view,SIGNAL(returnPressed()),this,SLOT(goToPage()));
}

SelectionTableView::~SelectionTableView()
{
	delete ui;
}
void SelectionTableView::refreshSelectedElementsTable(){
	CrossTimer crossTimer;
	QTableView* table = ui->tableView_selected_elements;
	std::unordered_map<int, vis::Element*> &elements = selection->getSelectedElements();
	maxPages = elements.size()/ITEMS_PER_PAGE+1;
	currentPage = std::min(currentPage,maxPages);
	currentPage = std::max(currentPage,1);
	switch(selection->getSelectionType()){
		case vis::CONSTANTS::VERTEX:{
			ui->label_elements_type->setText(QString("Vertex"));
			break;
		}
		case vis::CONSTANTS::POLYGON:{
			ui->label_elements_type->setText(QString("Polygon"));
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			ui->label_elements_type->setText(QString("Polyhedron"));
			break;
		}
		default:{
			ui->label_elements_type->setText(QString("None"));
			break;
		}
	}
	ui->label_number_ele->setText(QString::number(selection->getSelectedElements().size()));
	if(maxPages>1){
		this->ui->label_number_of_pages->setText(QString::number(maxPages));
		this->ui->lineEdit_selection_pages_view->setText(QString::number(currentPage));
		this->ui->pushButton_selection_page_back->setDisabled(this->currentPage==1);
		this->ui->pushButton_selection_page_next->setDisabled(this->currentPage==this->maxPages);
		this->ui->widget_selection_pages->show();
	}
	else{
		this->ui->widget_selection_pages->hide();
		this->currentPage = 1;
	}
	int currentPageStart = std::min(ITEMS_PER_PAGE*(currentPage-1),(int)elements.size());
	int currentPageElementsEnd = std::min(ITEMS_PER_PAGE*currentPage,(int)elements.size());
	QStandardItemModel *modelView = new QStandardItemModel(currentPageElementsEnd-currentPageStart,2,this); //2 Rows and 3 Columns
	modelView->setHorizontalHeaderItem(0, new QStandardItem(QString("Id")));
	modelView->setHorizontalHeaderItem(1, new QStandardItem(QString("Properties")));
	std::unordered_map<int, vis::Element*>::const_iterator It = elements.begin();

	int discard = 0;
	while(discard++<currentPageStart)
		It++;
	int start = 0;
	int end = currentPageElementsEnd-currentPageStart;
	while ( start<end )
	{
		QStandardItem *rowPos1 = new QStandardItem(QString::number(( *( *It ).second ).getId()));
		modelView->setItem(start,0,rowPos1);

		QStandardItem *rowPos2 = new QStandardItem(QString::fromStdString(ElementUtils::getPropertiesDisplay(It->second)));
		//rowPos2->setData(QString::fromStdString(ElementUtils::getPropertiesDisplay(It->second)),0);
		modelView->setItem(start,1,rowPos2);
		It++;
		start++;
	}
	table->setModel(modelView);
	std::cout << "Visualizador::refreshSelectedElementsTable(): " << crossTimer.getTranscurredSeconds() << " seconds" << std::endl;
}
void SelectionTableView::setSelection(Selection* s ){
	selection = s;
}
void SelectionTableView::nextPage(){
	this->currentPage++;
	this->currentPage = std::min(currentPage,maxPages);
	refreshSelectedElementsTable();
}

void SelectionTableView::lastPage(){
	this->currentPage--;
	this->currentPage = std::max(currentPage,1);
	refreshSelectedElementsTable();

}
void SelectionTableView::goToPage(){
	currentPage = QtUtils::readIntFromQText(ui->lineEdit_selection_pages_view->text(),currentPage);
	refreshSelectedElementsTable();
}
void SelectionTableView::refreshPage(){
	refreshSelectedElementsTable();
}
