#include "selectionstrategybypropertyconfig.h"
#include "ui_selectionstrategybypropertyconfig.h"
#include "Utils/qtutils.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Factories/EvaluationStrategyRegistry.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/Model.h"
SelectionStrategyByPropertyConfig::SelectionStrategyByPropertyConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SelectionStrategyByPropertyConfig)
{
	this->evaluationStrategyRegistry = EvaluationStrategyRegistry::getSingletonInstance();
	ui->setupUi(this);
	connect(ui->comboBoxEvalStrategiesSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeEvaluationStrategy(int)));
	this->fillEvaluationStrategiesComboBox();
	this->readConfig();

}
//Fill combobox with evaluation strategies
void SelectionStrategyByPropertyConfig::fillEvaluationStrategiesComboBox(){
	typedef std::map<float, unsigned char>::iterator it_type;
	int nitems = 0;
	for(it_type iterator = evaluationStrategyRegistry->getWeightMap()->begin();
		iterator != evaluationStrategyRegistry->getWeightMap()->end(); iterator++) {
		unsigned char key = iterator->second;
		EvaluationStrategy* p = evaluationStrategyRegistry->getRegistryByKeyInstance(key);
		if(p){
			this->ui->comboBoxEvalStrategiesSelect->addItem(p->getName(), QVariant((int)key));
			if(p->hasQIcon())
				ui->comboBoxEvalStrategiesSelect->setItemIcon(nitems,*p->getEvaluationStrategyQIcon());
			nitems++;
		}
	}
}

SelectionStrategyByPropertyConfig::~SelectionStrategyByPropertyConfig()
{
	delete ui;
}
void SelectionStrategyByPropertyConfig::readConfig(){
	minVal = QtUtils::readFloatFromQText(ui->ssbp_minimumValueInputSelect->text(),0.0f);
	maxVal = QtUtils::readFloatFromQText(ui->ssbp_maximumValueInputSelect->text(),1.0f);
	int index = this->ui->comboBoxEvalStrategiesSelect->currentIndex();
	int evalStrategyKey = this->ui->comboBoxEvalStrategiesSelect->itemData(index).toInt();
	evaluationStrategy = evaluationStrategyRegistry->getRegistryByKeyInstance(evalStrategyKey);

	applyOverModel = this->ui->radioApplyOverModel->isChecked();
	newSelection = this->ui->radioNewSelection->isChecked();
}
void SelectionStrategyByPropertyConfig::changeEvaluationStrategy(int index){
	int evalStrategyKey = this->ui->comboBoxEvalStrategiesSelect->itemData(index).toInt();
	EvaluationStrategy* evs = evaluationStrategyRegistry->getRegistryByKeyInstance(evalStrategyKey);
	if(evs){
		if(evs->getCalculatedValuesCount()==0){
			//model->evaluateUsingEvaluationStrategy(evs);
		}
		//ui->ssbp_minimumValueInputSelect->setText(QString::number(evs->getMinValue()));
		//ui->ssbp_maximumValueInputSelect->setText(QString::number(evs->getMaxValue()));
	}
}

void SelectionStrategyByPropertyConfig::setModel(RModel* rmodel) {
	this->model = rmodel->getOriginalModel();
}
