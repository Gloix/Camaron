#include "propertyvaluesrendererconfig.h"
#include "ui_propertyvaluesrendererconfig.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Factories/EvaluationStrategyRegistry.h"
#include "Utils/qtutils.h"

PropertyValuesRendererConfig::PropertyValuesRendererConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PropertyValuesRendererConfig)
{
	ui->setupUi(this);
	this->evaluationStrategyRegistry = EvaluationStrategyRegistry::getSingletonInstance();
	this->evaluationStrategy = (EvaluationStrategy*)0;
	this->fillEvaluationStrategiesComboBox();
	this->readConfig();
}
//Fill combobox with evaluation strategies
void PropertyValuesRendererConfig::fillEvaluationStrategiesComboBox(){
    typedef std::map<float, unsigned char>::iterator it_type;
    int nitems = 0;
    for(it_type iterator = evaluationStrategyRegistry->getWeightMap()->begin(); iterator != evaluationStrategyRegistry->getWeightMap()->end(); iterator++) {
        int key = iterator->second;
        EvaluationStrategy* p = evaluationStrategyRegistry->getRegistryByKeyInstance(key);
        if(!p||!p->hasStatics())
            continue;
        p->QApplicationInitiatedEv();
        this->ui->comboBox_evaluations->addItem(p->getName(), QVariant(key));
        if(p->hasQIcon())
            ui->comboBox_evaluations->setItemIcon(nitems,*p->getEvaluationStrategyQIcon());
        nitems++;
    }
}
PropertyValuesRendererConfig::~PropertyValuesRendererConfig()
{
	delete ui;
}
void PropertyValuesRendererConfig::readConfig(){
	this->drawOnlySelectedElements =  Qt::Checked == this->ui->checkBox_draw_only_selected->checkState();
	//min value color
	minValueColor.x = QtUtils::readFloatFromQText(ui->lineEdit_min_r->text(),0.0f);
	minValueColor.y = QtUtils::readFloatFromQText(ui->lineEdit_min_g->text(),0.0f);
	minValueColor.z = QtUtils::readFloatFromQText(ui->lineEdit_min_b->text(),0.0f);
	minValueColor.w = QtUtils::readFloatFromQText(ui->lineEdit_min_a->text(),1.0f);
	//max value color
	maxValueColor.x = QtUtils::readFloatFromQText(ui->lineEdit_max_r->text(),1.0f);
	maxValueColor.y = QtUtils::readFloatFromQText(ui->lineEdit_max_g->text(),1.0f);
	maxValueColor.z = QtUtils::readFloatFromQText(ui->lineEdit_max_b->text(),1.0f);
	maxValueColor.w = QtUtils::readFloatFromQText(ui->lineEdit_max_a->text(),1.0f);
	//no value color
	noValueColor.x = QtUtils::readFloatFromQText(ui->lineEdit_no_r->text(),1.0f);
	noValueColor.y = QtUtils::readFloatFromQText(ui->lineEdit_no_g->text(),1.0f);
	noValueColor.z = QtUtils::readFloatFromQText(ui->lineEdit_no_b->text(),0.0f);
	noValueColor.w = QtUtils::readFloatFromQText(ui->lineEdit_no_a->text(),1.0f);
	//Wire frame colors
	this->wireFrameColors.x = QtUtils::readFloatFromQText(ui->lineEdit_wf_r->text(),0.0f);
	this->wireFrameColors.y = QtUtils::readFloatFromQText(ui->lineEdit_wf_g->text(),0.0f);
	this->wireFrameColors.z = QtUtils::readFloatFromQText(ui->lineEdit_wf_b->text(),0.0f);
	this->wireFrameColors.w = QtUtils::readFloatFromQText(ui->lineEdit_wf_a->text(),1.0f);
	this->wireFrameLineWidth = QtUtils::readIntFromQText(ui->lineEdit_wf_w->text(),0);
	this->drawWireFrame =  Qt::Checked == this->ui->checkBox_MR_draw_w->checkState();

	//Wire frame selected colors
	this->wireFrameSelectedColors.x = QtUtils::readFloatFromQText(ui->lineEdit_sel_r->text(),1.0f);
	this->wireFrameSelectedColors.y = QtUtils::readFloatFromQText(ui->lineEdit_sel_g->text(),1.0f);
	this->wireFrameSelectedColors.z = QtUtils::readFloatFromQText(ui->lineEdit_sel_b->text(),0.0f);
	this->wireFrameSelectedColors.w = QtUtils::readFloatFromQText(ui->lineEdit_sel_a->text(),1.0f);
	this->wireFrameSelectedLineWidth = QtUtils::readIntFromQText(ui->lineEdit_sel_w->text(),2);
	//Evaluation Strategy
	int index = this->ui->comboBox_evaluations->currentIndex();
	int evalStrategyKey = this->ui->comboBox_evaluations->itemData(index).toInt();
	EvaluationStrategy* evaluationStrategySelected = evaluationStrategyRegistry->getRegistryByKeyInstance(evalStrategyKey);
	changedEvaluationStrategy = evaluationStrategySelected!=evaluationStrategy;
	evaluationStrategy = evaluationStrategySelected;
}
