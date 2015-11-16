#include "depthrendererconfig.h"
#include "ui_depthrendererconfig.h"
#include "Utils/qtutils.h"
DepthRendererConfig::DepthRendererConfig(QWidget *parent) :
	BaseRendererConfig(parent),
	ui(new Ui::DepthRendererConfig)
{
	ui->setupUi(this);
	connect(ui->comboBox_depth_func,SIGNAL( currentIndexChanged(int)),this, SLOT(changeDepthFunc(int)));
	GLenum enums[] = { GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL,
					   GL_EQUAL, GL_NOTEQUAL, GL_NEVER, GL_ALWAYS};
	std::string enumsNames[] = { "GL_LESS", "GL_LEQUAL","GL_GREATER","GL_GEQUAL",
								 "GL_EQUAL", "GL_NOTEQUAL",  "GL_NEVER","GL_ALWAYS"};
	for(int i = 0;i<8;i++){
		enumIdsMap.insert(std::make_pair(i,enums[i]));
		ui->comboBox_depth_func->addItem(QString::fromStdString(enumsNames[i]), QVariant(i));
	}
	depthFunc = GL_LESS;
}

DepthRendererConfig::~DepthRendererConfig()
{
	delete ui;
}
void DepthRendererConfig::readConfig(){
	modelBaseColor.x = QtUtils::readFloatFromQText(ui->lineEdit_MR_br->text(),1.0f);
	modelBaseColor.y = QtUtils::readFloatFromQText(ui->lineEdit_MR_bg->text(),1.0f);
	modelBaseColor.z = QtUtils::readFloatFromQText(ui->lineEdit_MR_bb->text(),1.0f);
}
void DepthRendererConfig::changeDepthFunc(int index){
	int key = this->ui->comboBox_depth_func->itemData(index).toInt();
	depthFunc = enumIdsMap[key];
}
