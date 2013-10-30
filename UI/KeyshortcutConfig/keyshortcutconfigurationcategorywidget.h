#ifndef KEYSHORTCUTCONFIGURATIONCATEGORYWIDGET_H
#define KEYSHORTCUTCONFIGURATIONCATEGORYWIDGET_H

#include <QWidget>
#include "Utils/userinputtranslationunit.h"
namespace Ui {
class KeyShortcutConfigurationCategoryWidget;
}

class KeyShortcutConfigurationCategoryWidget : public QWidget
{
		Q_OBJECT
		
	public:
		explicit KeyShortcutConfigurationCategoryWidget(std::string,
														std::vector<UserInputTranslationUnit>&,
														QWidget *parent = 0);
		~KeyShortcutConfigurationCategoryWidget();
		
	private:
		Ui::KeyShortcutConfigurationCategoryWidget *ui;
		std::vector<UserInputTranslationUnit>& translationUnits;
};

#endif // KEYSHORTCUTCONFIGURATIONCATEGORYWIDGET_H
