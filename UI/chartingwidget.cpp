#include "chartingwidget.h"
#include <QPainter>
#include <QToolTip>
#include <QHelpEvent>
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
ChartingWidget::ChartingWidget(QWidget *parent) :
	QWidget(parent),
	colorDialog(),
	chartColor(Qt::green)
{
	ss << std::setprecision(std::numeric_limits<float>::digits10+1);
	connect(&colorDialog,SIGNAL(colorSelected(QColor)),this,SLOT(colorChanged(QColor)));
	unusedSpace = 0;
}
void ChartingWidget::colorChanged(QColor qcolor){
	chartColor = qcolor;
	repaint();
}

void ChartingWidget::mousePressEvent(QMouseEvent *event){
	if (event->buttons() & Qt::LeftButton){
		colorDialog.show();
	}
}

bool ChartingWidget::event(QEvent *event)
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		int x = helpEvent->pos().x();
		int y = helpEvent->pos().y();
		std::cout << "X: " << x << ", Y:" << y << std::endl;
		int index = getBarTooltip(x,y);
		if (index != -1) {
			QToolTip::showText(helpEvent->globalPos(),countData[index].tooltip);

		} else {
			event->ignore();
		}
		return true;
	}
	return QWidget::event(event);

}

void ChartingWidget::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.setBrush(chartColor);
	for(std::vector<int>::size_type i = 0; i < countData.size();i ++)
		this->drawData(countData[i], i,&painter);

	//painter.setPen(pen);
	//painter.setBrush(brush);
	//painter.restore();

	//painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
void ChartingWidget::drawData(BarShape& c, int pos, QPainter* painter){
	int barWidth = width()/countData.size();
	unusedSpace = width()-(countData.size()*barWidth);
	float heightRatio = ((float)c.count)/((float)maxCount);
	int barHeight = height()*heightRatio -1;
	int posX = pos*barWidth + unusedSpace/2;
	painter->drawRect(QRect(posX,height()-barHeight,barWidth,barHeight));
}
int ChartingWidget::getBarTooltip(int x, int y){
	if(countData.size()==0)
		return -1;
	if(x<unusedSpace/2||x>(width()-unusedSpace/2))
		return -1;
	float ratio = ((float)(x-unusedSpace/2))/((float)(width()-unusedSpace));
	return (int)countData.size()*ratio;
}

void ChartingWidget::setCountData(std::vector<BarShape>& counts){
	countData.clear();
	countData.reserve(counts.size());
	if(counts.size()==0)
		return;
	minCount = maxCount = counts[0].count;
	for( BarShape barShape : counts ){
		minCount = std::min(minCount, barShape.count);
		maxCount = std::max(maxCount, barShape.count);
		ss << "Count: " << barShape.count << "\n";
		ss << "Range: [" << barShape.rangeMin << ", " << barShape.rangeMax <<"]\n";
		ss << "Percent: " << barShape.percent << "%\n ";
		countData.push_back(BarShape(barShape.count,barShape.rangeMin,barShape.rangeMax,barShape.percent,QString(ss.str().c_str())));
		ss.str(std::string());
	}
}
void ChartingWidget::clearCountData(){
	countData.clear();
}
