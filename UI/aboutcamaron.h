#ifndef ABOUTCAMARON_H
#define ABOUTCAMARON_H

#include <QWidget>

namespace Ui {
class AboutCamaron;
}

class AboutCamaron : public QWidget
{
		Q_OBJECT
		
	public:
		explicit AboutCamaron(QWidget *parent = 0);
		~AboutCamaron();
		
	private:
		Ui::AboutCamaron *ui;
};

#endif // ABOUTCAMARON_H
