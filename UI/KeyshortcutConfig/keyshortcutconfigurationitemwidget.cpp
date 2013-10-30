#include "keyshortcutconfigurationitemwidget.h"
#include "ui_keyshortcutconfigurationitemwidget.h"
#include <QInputDialog>
KeyShortcutConfigurationItemWidget::KeyShortcutConfigurationItemWidget(UserInputTranslationUnit &u,
																	   QWidget *parent) :
	QWidget(parent),
	ui(new Ui::KeyShortcutConfigurationItemWidget),
	unit(u)
{
	ui->setupUi(this);
	ui->label_name->setText(QString::fromStdString(unit.getDescription()));
//	connect(ui->pushButton_change,SIGNAL(clicked()),this,SLOT(changeShortCut()));
	setLabelsTextAndCheckboxState();
}

KeyShortcutConfigurationItemWidget::~KeyShortcutConfigurationItemWidget()
{
	delete ui;
}
void KeyShortcutConfigurationItemWidget::setLabelsTextAndCheckboxState(){
	if(unit.isKeyValid())
		ui->label_key->setText(QKeySequence(unit.getKey()).toString());
	else
		ui->label_key->setText(QString(""));
	if(unit.isMouseValid())
		ui->label_mouse->setText(mouseButtonToQString(unit.getMouseButton(),unit.getWheel()));
	else
		ui->label_mouse->setText(QString(""));
	ui->checkBox_cntrl->setChecked(unit.isControl());
	ui->checkBox_shift->setChecked(unit.isShift());
}

void KeyShortcutConfigurationItemWidget::changeShortCut(){
	QInputDialog p;
	p.exec();
}


QString KeyShortcutConfigurationItemWidget::mouseButtonToQString(Qt::MouseButton b,int c){
	switch(b){
		case Qt::LeftButton: return QString("Left");
		case Qt::RightButton: return QString("Right");
		case Qt::MiddleButton: return QString("Middle");
		default:;
	}
	switch(c){
		case UserInputTranslationUnit::DOWN_MOUSE_WHEEL: return QString("Wheel Down");
		case UserInputTranslationUnit::UP_MOUSE_WHEEL: return QString("Wheel Up");
		default: return QString("No Button");
	}
}
