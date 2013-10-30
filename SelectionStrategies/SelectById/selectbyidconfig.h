#ifndef SELECTBYIDCONFIG_H
#define SELECTBYIDCONFIG_H

#include <QWidget>

namespace Ui {
class SelectByIdConfig;
}

class SelectByIdConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit SelectByIdConfig(QWidget *parent = 0);
		~SelectByIdConfig();
		void readConfig();
		static const int NEW_SELECTION = 0;
		static const int JOIN_SELECTION = 1;
		static const int INTERSECT_SELECTION = 2;
		int elementsType;//polygon, vertex or polyhedron
		int selectionType;
		int minId,maxId;
	private:
		Ui::SelectByIdConfig *ui;
};

#endif // SELECTBYIDCONFIG_H
