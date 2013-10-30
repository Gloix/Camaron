#ifndef KEYSHORTCUTCONFIGURATION_H
#define KEYSHORTCUTCONFIGURATION_H

#include <QWidget>

namespace Ui {
class KeyShortcutConfiguration;
}

class KeyShortcutConfiguration : public QWidget
{
		Q_OBJECT
		
	public:
		explicit KeyShortcutConfiguration(QWidget *parent = 0);
		~KeyShortcutConfiguration();
	private:
		Ui::KeyShortcutConfiguration *ui;
};

#endif // KEYSHORTCUTCONFIGURATION_H
