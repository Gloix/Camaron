#include "keyshortcutconfigurationcategorywidget.h"
#include "ui_keyshortcutconfigurationcategorywidget.h"
#include "UI/KeyshortcutConfig/keyshortcutconfigurationitemwidget.h"

KeyShortcutConfigurationCategoryWidget::KeyShortcutConfigurationCategoryWidget(std::string categoryname,
																			   std::vector<UserInputTranslationUnit> &units,
																			   QWidget *parent) :
	QWidget(parent),
	ui(new Ui::KeyShortcutConfigurationCategoryWidget),
	translationUnits(units)
{
	ui->setupUi(this);
	ui->label_cat_name->setText(QString::fromStdString(categoryname));
	for(std::vector<UserInputTranslationUnit>::size_type i = 0;
		i<translationUnits.size();i++){
		KeyShortcutConfigurationItemWidget* unit;
		unit = new KeyShortcutConfigurationItemWidget(translationUnits[i],this);
		ui->widget_unit_container->layout()->addWidget(unit);
	}
}

KeyShortcutConfigurationCategoryWidget::~KeyShortcutConfigurationCategoryWidget()
{
	delete ui;
}
