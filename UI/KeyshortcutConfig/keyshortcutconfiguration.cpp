#include "keyshortcutconfiguration.h"
#include "ui_keyshortcutconfiguration.h"
#include "Utils/userinputtranslator.h"
#include "UI/KeyshortcutConfig/keyshortcutconfigurationcategorywidget.h"
KeyShortcutConfiguration::KeyShortcutConfiguration(QWidget *parent) :
	QWidget(parent,
			Qt::Tool|
			Qt::Window |
			Qt::CustomizeWindowHint|
			Qt::WindowMinimizeButtonHint|
			Qt::WindowCloseButtonHint),
	ui(new Ui::KeyShortcutConfiguration)
{
	ui->setupUi(this);
	UserInputTranslator* instance = UserInputTranslator::getSingletonInstance();
	std::map<std::string,std::vector<UserInputTranslationUnit> >& units =
			instance->getDictionary();
	typedef std::map<std::string,std::vector<UserInputTranslationUnit> >::iterator it_type;
	for(it_type it = units.begin();it!=units.end();it++){
		KeyShortcutConfigurationCategoryWidget* widgetToAdd;
		widgetToAdd = new KeyShortcutConfigurationCategoryWidget(it->first,it->second);
		ui->verticalLayout->addWidget(widgetToAdd);
	}
}

KeyShortcutConfiguration::~KeyShortcutConfiguration()
{
	delete ui;
	UserInputTranslator::getSingletonInstance()->saveConfigToFile("bla");
	UserInputTranslator::deleteSingletonInstance();
}
