#include "baserendererconfig.h"

BaseRendererConfig::BaseRendererConfig(QWidget *parent) : QWidget(parent)
{

}

BaseRendererConfig::forceUpdate() {
    emit onConfigForceApply();
}

//void BaseRendererConfig::setRendererConfigPopup(RendererConfigPopUp rendererConfigPopUp) {
//    this->rendererConfigPopUp = rendererConfigPopUp;
//}

