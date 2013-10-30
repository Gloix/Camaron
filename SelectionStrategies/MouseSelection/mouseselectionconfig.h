#ifndef MOUSESELECTIONCONFIG_H
#define MOUSESELECTIONCONFIG_H

#include <QWidget>

namespace Ui {
class MouseSelectionConfig;
}

class MouseSelectionConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit MouseSelectionConfig(QWidget *parent = 0);
		~MouseSelectionConfig();
		void readConfig();
		static const int NEW_SELECTION = 0;
		static const int JOIN_SELECTION = 1;
		static const int INTERSECT_SELECTION = 2;
		static const int INVERT_SELECTION = 3;
		bool onlySurface, onlyInterior, onlyFront, onlyBack,closestToCamera;
		int elementsType;//polygon, vertex or polyhedron
		int selectionType;
		float pixelTolerance;

	private:
		Ui::MouseSelectionConfig *ui;
};

#endif // MOUSESELECTIONCONFIG_H
