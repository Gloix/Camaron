#include "clickableqlabel.h"
ClickableLabel::ClickableLabel( const QString& text, QWidget * parent ) :
	QLabel(parent)

{
	this->setText(text);
	this->setAlignment(Qt::AlignHCenter);
	this->setStyleSheet("ClickableLabel { background-color :lightblue; border: 3px solid grey; border-radius: 3px;}");
}


ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent ( QMouseEvent * )

{
	emit clicked();
}
