#include "modelgeneralstaticswidget.h"
#include "ui_modelgeneralstaticswidget.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include <iomanip>
#include <limits>
#include <sstream>
ModelGeneralStaticsWidget::ModelGeneralStaticsWidget(QWidget *parent) :
	QWidget(parent,Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint ),
	ui(new Ui::ModelGeneralStaticsWidget)
{
	ui->setupUi(this);
	clearData();
}

ModelGeneralStaticsWidget::~ModelGeneralStaticsWidget()
{
	delete ui;
}

void ModelGeneralStaticsWidget::setData(ModelGeneralStaticsCollector* m){
	std::stringstream content;
	content << std::setprecision(std::numeric_limits<float>::digits10);
	std::vector<ModelStaticsItem>& modelStaticsItems = m->getModelStatics();
	for(std::vector<ModelStaticsItem>::size_type i = 0;
		i<modelStaticsItems.size();i++){
		content << "<h4>" << modelStaticsItems[i].itemTitle << "</h4>";
		std::map<std::string, std::string>& stats = modelStaticsItems[i].stats;
		for(std::map<std::string, std::string>::iterator j = stats.begin();
			j!=stats.end();j++){
			content << j->first << " " << j->second << "<br/>";
		}
	}
	ui->textBrowser->setText(QString::fromStdString(content.str()));
}

void ModelGeneralStaticsWidget::clearData(){
	ui->textBrowser->setText(QString("No model Loaded"));
}
