#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H
#include <QLabel>
class ClickableLabel : public QLabel
{

		Q_OBJECT

	public:
		explicit ClickableLabel( const QString& text ="", QWidget * parent = 0 );
		~ClickableLabel();

	signals:
		void clicked();

	protected:
		void mousePressEvent ( QMouseEvent * event ) ;
};
#endif // CLICKABLEQLABEL_H
