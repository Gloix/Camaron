#ifndef ELEMENTDRAWNOPTIONCONFIG_H
#define ELEMENTDRAWNOPTIONCONFIG_H

#include <QWidget>

namespace Ui {
class ElementDrawnOptionConfig;
}

class ElementDrawnOptionConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit ElementDrawnOptionConfig(QWidget *parent = 0);
		~ElementDrawnOptionConfig();

		void readConfig();
		int elementDrawnOption;
		static const int DRAW_ALL = 0;
		static const int DRAW_ONLY_SURFACE = 1;
		static const int DRAW_ONLY_INTERIOR = 2;
		static const int DRAW_ONLY_SELECTED = 3;
		static const int DRAW_ONLY_UNSELECTED = 4;
	private:
		Ui::ElementDrawnOptionConfig *ui;
};

#endif // ELEMENTDRAWNOPTIONCONFIG_H
