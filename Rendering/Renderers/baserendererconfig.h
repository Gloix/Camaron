#ifndef BASERENDERERCONFIG_H
#define BASERENDERERCONFIG_H

#include <QWidget>
//#include "rendererconfigpopup.h"

class BaseRendererConfig : public QWidget
{
    Q_OBJECT
    public:
        explicit BaseRendererConfig(QWidget *parent = 0);
        //void setRendererConfigPopup(RendererConfigPopUp rendererConfigPopUp);
    protected:
        forceUpdate();

    signals:
        void onConfigForceApply();

    public slots:

    private:
        //RendererConfigPopUp rendererConfigPopUp;
};

#endif // BASERENDERERCONFIG_H
