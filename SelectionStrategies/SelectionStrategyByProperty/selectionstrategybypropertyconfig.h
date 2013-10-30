#ifndef SELECTIONSTRATEGYBYPROPERTYCONFIG_H
#define SELECTIONSTRATEGYBYPROPERTYCONFIG_H

#include <QWidget>
namespace Ui {
class SelectionStrategyByPropertyConfig;
}
class EvaluationStrategyRegistry;
class EvaluationStrategy;
class SelectionStrategyByPropertyConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit SelectionStrategyByPropertyConfig(QWidget *parent = 0);
		virtual ~SelectionStrategyByPropertyConfig();

		void readConfig();
		float minVal;
		float maxVal;
		EvaluationStrategy* evaluationStrategy;
		bool applyOverModel;//false = overSelection
		bool newSelection;//false = join
	public slots:
		void changeEvaluationStrategy(int index);
	private:
		EvaluationStrategyRegistry* evaluationStrategyRegistry;
		Ui::SelectionStrategyByPropertyConfig *ui;
		void fillEvaluationStrategiesComboBox();
};

#endif // SELECTIONSTRATEGYBYPROPERTYCONFIG_H
