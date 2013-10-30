#include "genericconfigwidget.h"

GenericConfigWidget::GenericConfigWidget(std::string nname):
	name(nname)
{
}

GenericConfigWidget::~GenericConfigWidget()
{
}
std::string GenericConfigWidget::getName(){
	return name;
}
