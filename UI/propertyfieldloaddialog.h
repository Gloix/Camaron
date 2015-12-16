#ifndef PROPERTYFIELDLOADDIALOG_H
#define PROPERTYFIELDLOADDIALOG_H

#include <QWidget>
#include <string>
#include <memory>

namespace Ui {
class PropertyFieldLoadDialog;
}
class PropertyFieldDef;

class PropertyFieldLoadDialog : public QWidget
{
		Q_OBJECT
		
	public:
		explicit PropertyFieldLoadDialog(QWidget *parent = 0);
		~PropertyFieldLoadDialog();
		void setPropertyFieldName(std::string propertyFieldName);
		void setupForPropertyFields(std::vector<std::shared_ptr<PropertyFieldDef>> propertyFieldDefs);
	public slots:
		void stageComplete(PropertyFieldDef*);
		void addMessage(QString message);
		void displayError(QString message);
		void displayWarning(QString message);
	private:
		Ui::PropertyFieldLoadDialog *ui;
		void updateProgressBar();
		//int progressBarValue;
		int progressStages;
		//int progressVarMax;
		int modelType;
};

#endif // PROPERTYFIELDLOADDIALOG_H
