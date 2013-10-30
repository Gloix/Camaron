#ifndef MODELGENERALSTATICSWIDGET_H
#define MODELGENERALSTATICSWIDGET_H

#include <QWidget>

namespace Ui {
class ModelGeneralStaticsWidget;
}
class ModelGeneralStaticsCollector;
class ModelGeneralStaticsWidget : public QWidget
{
		Q_OBJECT
		
	public:
		explicit ModelGeneralStaticsWidget(QWidget *parent = 0);
		~ModelGeneralStaticsWidget();
		void setData(ModelGeneralStaticsCollector*);
		void clearData();
	private:
		Ui::ModelGeneralStaticsWidget *ui;
};

#endif // MODELGENERALSTATICSWIDGET_H
