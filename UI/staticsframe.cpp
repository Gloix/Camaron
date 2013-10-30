#include "staticsframe.h"
#include "ui_staticsframe.h"
#include "Factories/EvaluationStrategyRegistry.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Utils/qtutils.h"
#include <iostream>
StaticsFrame::StaticsFrame(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint ),
	ui(new Ui::StaticsFrame)
{
	ui->setupUi(this);
	chartingWidget = new ChartingWidget(this->ui->widget_chart_container);
	this->ui->widget_chart_container->layout()->addWidget(this->chartingWidget);
	evaluationStrategy = (EvaluationStrategy*)0;
	evaluationStrategyRegistry = EvaluationStrategyRegistry::getSingletonInstance();
	connect(this->ui->pushButton_refresh_stat,SIGNAL(clicked()),this,SLOT(refreshStatics()));
	connect(this->ui->comboBox_statics_eval_strategies,SIGNAL(currentIndexChanged(int)),this,SLOT(changeEvaluationStrategy(int)));
}

StaticsFrame::~StaticsFrame()
{
	delete ui;
}

//Fill combobox with evaluation strategies
void StaticsFrame::fillEvaluationStrategiesComboBox(){
    typedef std::map<float, unsigned char>::iterator it_type;
    int nitems = 0;
    for(it_type iterator = evaluationStrategyRegistry->getWeightMap()->begin(); iterator != evaluationStrategyRegistry->getWeightMap()->end(); iterator++) {
        int key = iterator->second;
        EvaluationStrategy* p = evaluationStrategyRegistry->getRegistryByKeyInstance(key);
        if(p->hasStatics()){
            p->QApplicationInitiatedEv();
            this->ui->comboBox_statics_eval_strategies->addItem(p->getName(), QVariant((int)key));
            if(p->hasQIcon())
                ui->comboBox_statics_eval_strategies->setItemIcon(nitems,*p->getEvaluationStrategyQIcon());
            nitems++;
        }
    }
}

void StaticsFrame::refreshStatics(){
	if(!evaluationStrategy)
		return;
	int step = QtUtils::readIntFromQText(ui->lineEdit_step->text(),100);
	float from = QtUtils::readFloatFromQText(ui->lineEdit_stat_from->text(),0.0f);
	float to = QtUtils::readFloatFromQText(ui->lineEdit_stat_to->text(),1.0f);
	if(from>=to || step == 0)
		return;
	float rangeLenght = (to-from)/step;
	std::vector<BarShape> shapes;
	shapes.reserve(step);
	for(int i = 0;i<step;i++){
		float fromRange = from + rangeLenght*i;
		float toRange = to;
		if(i!=(step-1))
			toRange = from + rangeLenght*(i+1);
		int count = evaluationStrategy->getValuesCountInRange(fromRange,toRange);
		shapes.push_back(BarShape(count,fromRange,toRange,((float)count)/((float)evaluationStrategy->getCalculatedValuesCount())*100.0));
	}
	chartingWidget->setCountData(shapes);
	chartingWidget->repaint();
}

void StaticsFrame::changeEvaluationStrategy(int index){
	int key = this->ui->comboBox_statics_eval_strategies->itemData(index).toInt();
	evaluationStrategy = this->evaluationStrategyRegistry->getRegistryByKeyInstance(key);
	this->refreshEvaluationStrategyData();
	chartingWidget->repaint();
}
void StaticsFrame::refreshEvaluationStrategyData(){
	if(!evaluationStrategy)
		return;
	if(evaluationStrategy->getCalculatedValuesCount()>0){
		ui->label_maximum_val->setText(QString::number(evaluationStrategy->getMaxValue()));
		ui->label_minimum_val->setText(QString::number(evaluationStrategy->getMinValue()));
		ui->lineEdit_stat_to->setText(QString::number(evaluationStrategy->getMaxValue()));
		ui->lineEdit_stat_from->setText(QString::number(evaluationStrategy->getMinValue()));
		ui->pushButton_refresh_stat->setDisabled(false);
	}
	else{
		ui->label_maximum_val->setText("No data.");
		ui->label_minimum_val->setText("No data.");
		ui->pushButton_refresh_stat->setDisabled(true);
	}
	ui->label_total_elements->setText(QString::number(evaluationStrategy->getTotalElementsCount()));
	ui->label_evaluated_elements->setText(QString::number(evaluationStrategy->getCalculatedValuesCount()));
}
void StaticsFrame::refreshAndShow(){
	this->refreshEvaluationStrategyData();
	this->show();
}
