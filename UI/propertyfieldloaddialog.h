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
		void setupForPropertyFields(std::string filename, std::vector<std::shared_ptr<PropertyFieldDef>> propertyFieldDefs);
	signals:
		void onReadyToLoad(std::string, std::vector<std::shared_ptr<PropertyFieldDef>>);
	private slots:
		void onLoadClick(bool);
		void onCloseClick(bool);
	public slots:
		void setLoadedProgress(unsigned int);
		void addMessage(QString message);
		void displayError(QString message);
		void displayWarning(QString message);
	private:
		std::vector<std::shared_ptr<PropertyFieldDef>> propsList;
		std::string filename;
		Ui::PropertyFieldLoadDialog *ui;
		void updateProgressBar();
		//int progressBarValue;
		unsigned int progress;
		//int progressStages;
		//int progressVarMax;
		int modelType;
};

#endif // PROPERTYFIELDLOADDIALOG_H
