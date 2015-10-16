#ifndef RENDERERCONFIGPOPUP_H
#define RENDERERCONFIGPOPUP_H

#include <QWidget>
#include "Rendering/Renderers/baserendererconfig.h"

namespace Ui {
class RendererConfigPopUp;
}

class RendererConfigPopUp : public QWidget
{
		Q_OBJECT
		
	public:
		explicit RendererConfigPopUp(QWidget *parent = 0);
		~RendererConfigPopUp();
        void setConfigQWidget(BaseRendererConfig*);
		void cleanConfigQWidget();
	public slots:
		void applyChangesPushButtonClicked();
	signals:
		void applyChangesPushButton();
	private:
		Ui::RendererConfigPopUp *ui;
};

#endif // RENDERERCONFIGPOPUP_H
