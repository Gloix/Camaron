#ifndef RENDERERCONFIGPOPUP_H
#define RENDERERCONFIGPOPUP_H

#include <QWidget>

namespace Ui {
class RendererConfigPopUp;
}

class RendererConfigPopUp : public QWidget
{
		Q_OBJECT
		
	public:
		explicit RendererConfigPopUp(QWidget *parent = 0);
		~RendererConfigPopUp();
		void setConfigQWidget(QWidget*);
		void cleanConfigQWidget();
	public slots:
		void applyChangesPushButtonClicked();
	signals:
		void applyChangesPushButton();
	private:
		Ui::RendererConfigPopUp *ui;
};

#endif // RENDERERCONFIGPOPUP_H
