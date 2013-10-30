#ifndef EXPANDTONEIGHBORSCONFIG_H
#define EXPANDTONEIGHBORSCONFIG_H

#include <QWidget>

namespace Ui {
class ExpandToNeighborsConfig;
}

class ExpandToNeighborsConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit ExpandToNeighborsConfig(QWidget *parent = 0);
		~ExpandToNeighborsConfig();
		void readConfig();
		bool onlySurface;
		bool onlySurfaceWithAngle;
		float dihedralAngle;
	private:
		Ui::ExpandToNeighborsConfig *ui;
};

#endif // EXPANDTONEIGHBORSCONFIG_H
