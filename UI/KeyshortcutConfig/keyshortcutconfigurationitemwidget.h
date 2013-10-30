#ifndef KEYSHORTCUTCONFIGURATIONITEMWIDGET_H
#define KEYSHORTCUTCONFIGURATIONITEMWIDGET_H

#include <QWidget>
#include "Utils/userinputtranslationunit.h"
namespace Ui {
class KeyShortcutConfigurationItemWidget;
}

class KeyShortcutConfigurationItemWidget : public QWidget
{
		Q_OBJECT
		
	public:
		explicit KeyShortcutConfigurationItemWidget(UserInputTranslationUnit&,
													QWidget *parent = 0);
		~KeyShortcutConfigurationItemWidget();
	public slots:
		void changeShortCut();
	private:
		Ui::KeyShortcutConfigurationItemWidget *ui;
		UserInputTranslationUnit& unit;
		void setLabelsTextAndCheckboxState();
		QString mouseButtonToQString(Qt::MouseButton, int c);
};

#endif // KEYSHORTCUTCONFIGURATIONITEMWIDGET_H
