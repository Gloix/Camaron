#ifndef BASERENDERERCONFIG_H
#define BASERENDERERCONFIG_H

#include <QWidget>
//#include "rendererconfigpopup.h"
#include <memory>

class ScalarFieldDef;
class TQSPropertyFieldDef;

class BaseRendererConfig : public QWidget
{
	Q_OBJECT
	public:
		explicit BaseRendererConfig(QWidget *parent = 0);
		//void setRendererConfigPopup(RendererConfigPopUp rendererConfigPopUp);
//		virtual void addPropertyFieldDef(std::shared_ptr<ScalarFieldDef> pfd);
//		virtual void addPropertyFieldDef(std::shared_ptr<TQSPropertyFieldDef> pfd);
	protected:
		void forceUpdate();

	signals:
		void onConfigForceApply();

	public slots:

	private:
		//RendererConfigPopUp rendererConfigPopUp;
};

#endif // BASERENDERERCONFIG_H
