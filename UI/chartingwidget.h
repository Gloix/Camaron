#ifndef CHARTINGWIDGET_H
#define CHARTINGWIDGET_H

#include <QWidget>
#include <QString>
#include <sstream>
#include <QColorDialog>
struct BarShape{
	public:
		BarShape(int c, float from, float to, float p, QString toolTipText = ""):
			tooltip(toolTipText){
			count = c;
			percent = p;
			rangeMin = from;
			rangeMax = to;
		}
		QString tooltip;
		int count;
		float percent;
		float rangeMin;
		float rangeMax;
};

class ChartingWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit ChartingWidget(QWidget *parent = 0);
		void setCountData(std::vector<BarShape>&);
		void clearCountData();
	signals:
		
	public slots:
		void colorChanged(QColor);
	protected:
		void mousePressEvent(QMouseEvent *event);
		void paintEvent(QPaintEvent * /* event */);
		bool event(QEvent *event);
	private:
		void drawData(BarShape& c,int pos, QPainter*);
		int getBarTooltip(int x, int y);
		std::vector<BarShape> countData;
		float min;
		float max;
		int minCount;
		int maxCount;
		QColorDialog colorDialog;
		std::stringstream ss;
		QColor chartColor;
		int unusedSpace;

		
};

#endif // CHARTINGWIDGET_H
