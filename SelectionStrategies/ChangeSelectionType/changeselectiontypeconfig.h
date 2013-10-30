#ifndef CHANGESELECTIONTYPECONFIG_H
#define CHANGESELECTIONTYPECONFIG_H

#include <QWidget>

namespace Ui {
class ChangeSelectionTypeConfig;
}

class ChangeSelectionTypeConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit ChangeSelectionTypeConfig(QWidget *parent = 0);
		~ChangeSelectionTypeConfig();
		void readConfig();
		int elementsType;//polygon, vertex or polyhedron
		bool hint;//true == all elements must be selected
		
	private:
		Ui::ChangeSelectionTypeConfig *ui;

};

#endif // CHANGESELECTIONTYPECONFIG_H
