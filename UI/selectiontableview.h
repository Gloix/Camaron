#ifndef SELECTIONTABLEVIEW_H
#define SELECTIONTABLEVIEW_H

#include <QWidget>

namespace Ui {
class SelectionTableView;
}
class Selection;
class SelectionTableView : public QWidget
{
		Q_OBJECT
		
	public:
		explicit SelectionTableView(QWidget *parent = 0);
		void refreshSelectedElementsTable();
		void setSelection(Selection*);
		~SelectionTableView();
	public slots:
		void nextPage();
		void lastPage();
		void goToPage();
		void refreshPage();
	private:
		Ui::SelectionTableView *ui;
		Selection* selection;
		int currentPage;
		int maxPages;
};

#endif // SELECTIONTABLEVIEW_H
