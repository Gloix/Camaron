#include "baserendererconfig.h"

BaseRendererConfig::BaseRendererConfig(QWidget *parent) : QWidget(parent)
{

}

void BaseRendererConfig::forceUpdate() {
	emit onConfigForceApply();
}

//void BaseRendererConfig::addPropertyFieldDef(std::shared_ptr<ScalarFieldDef> pfd) {
//}

//void BaseRendererConfig::addPropertyFieldDef(std::shared_ptr<TQSPropertyFieldDef> pfd) {
//}

//void BaseRendererConfig::setRendererConfigPopup(RendererConfigPopUp rendererConfigPopUp) {
//	this->rendererConfigPopUp = rendererConfigPopUp;
//}

