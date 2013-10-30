#ifndef GENERICCONFIGWIDGET_H
#define GENERICCONFIGWIDGET_H
#include <string>
class QWidget;

class GenericConfigWidget
{
	public:
		GenericConfigWidget(std::string name);
		virtual ~GenericConfigWidget();
		virtual void readGenericConfig() = 0;
		virtual QWidget* getGenericWidget() = 0;
		std::string getName();
	private:
		std::string name;
};

#endif // GENERICCONFIGWIDGET_H
