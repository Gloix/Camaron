#ifndef STATICSFRAME_H
#define STATICSFRAME_H

#include <QWidget>
#include "UI/chartingwidget.h"
class EvaluationStrategyRegistry;
class EvaluationStrategy;
namespace Ui {
class StaticsFrame;
}

class StaticsFrame : public QWidget
{
		Q_OBJECT
		
	public:
		explicit StaticsFrame(QWidget *parent = 0);
		~StaticsFrame();
		void fillEvaluationStrategiesComboBox();
	public slots:
		void refreshStatics();
		void changeEvaluationStrategy(int);
		void refreshAndShow();
	private:
		void refreshEvaluationStrategyData();
		Ui::StaticsFrame *ui;
		EvaluationStrategyRegistry* evaluationStrategyRegistry;
		EvaluationStrategy* evaluationStrategy;
		ChartingWidget* chartingWidget;
};

#endif // STATICSFRAME_H
