#include "convexgeometryintersectionrendererconfig.h"
#include "ui_convexgeometryintersectionrendererconfig.h"
#include "Rendering/Renderers/ModelMainRenderer/mainrendererconfig.h"
#include "Utils/fileutils.h"
#include "Utils/qtutils.h"
#include <QFileDialog>
#include "UI/clickableqlabel.h"
ConvexGeometryIntersectionRendererConfig::ConvexGeometryIntersectionRendererConfig(QWidget *parent) :
	QWidget(parent),
	//mainRendererConfig(this),
	ui(new Ui::ConvexGeometryIntersectionRendererConfig)
{
	ui->setupUi(this);
	mainRendererConfig = new MainRendererConfig(this);
	ui->widget_mm_wid->layout()->addWidget(mainRendererConfig);
	//Clickable Labels
	ClickableLabel* polygonFomatLabel = new ClickableLabel(QString("Polygons"),this);
	polygonFomatLabel->setToolTip(QString("POLYGONS\n<N-Vertices>\n<V0-X> <V0-Y> <V0-Z>\n...\n<V(N-1)-X> <V(N-1)-Y> <V(N-1)-Z>\n<M-Polygons>\n<P0-k-Vertices> <P0-V0ID> ... <P0-V(k-1)ID>\n...\n<P(M-1)-j-Vertices> <P(M-1)-V0ID>...<P(M-1)-V(j-1)ID>"));
	ClickableLabel* sphereFomatLabel = new ClickableLabel(QString("Sphere"),this);
	sphereFomatLabel->setToolTip(QString("SPHERE\n <Radio>\n <X-Center>\n <Y-Center>\n <Z-Center> \n <N-Triangles>, <N-Triangles> is an optional parameter."));
	ClickableLabel* normalsFomatLabel = new ClickableLabel(QString("Normals"),this);
	normalsFomatLabel->setToolTip(QString("NORMALS\n<M-Normals>\n<N0-Tail-X> <N0-Tail-Y> <N0-Tail-Z> <N0-HEADR-X> <N0-HEADR-X> <N0-HEADR-Z>\n...\n<N(M-1)-Tail-X> <N(M-1)-Tail-Y> <N(M-1)-Tail-Z> <N(M-1)-HEADR-X> <N(M-1)-HEADR-X> <N(M-1)-HEADR-Z>,\nTail coords are related to the model coordinates system and Head coords are related to the Tail"));
	ui->widget_7_formats->layout()->addWidget(polygonFomatLabel);
	ui->widget_7_formats->layout()->addWidget(sphereFomatLabel);
	ui->widget_7_formats->layout()->addWidget(normalsFomatLabel);
	connect(polygonFomatLabel,SIGNAL(clicked()),this,SLOT(changeTextToPolygonsFormat()));
	connect(sphereFomatLabel,SIGNAL(clicked()),this,SLOT(changeTextToSphereFormat()));
	connect(normalsFomatLabel,SIGNAL(clicked()),this,SLOT(changeTextToNormalsFormat()));
	connect(ui->pushButton_file,SIGNAL(clicked()),this,SLOT(getFromFile()));
	connect(ui->pushButton_save,SIGNAL(clicked()),this,SLOT(saveToFile()));
	connect(ui->plainTextEdit,SIGNAL(textChanged()),this,SLOT(textChangedChangeBool()));
	readConfig();
	changedFollowModel = true;
	mainRendererConfig->depthPeelingNotAvailable();
	textChangedChangeBool();
}

ConvexGeometryIntersectionRendererConfig::~ConvexGeometryIntersectionRendererConfig()
{
	delete ui;
}
void ConvexGeometryIntersectionRendererConfig::readConfig(){
	mainRendererConfig->readConfig();
	bool oldFollowModel = followModel;
	followModel = ui->checkBox_move_with_model->isChecked();
	changedFollowModel = followModel != oldFollowModel;
	geometryText = ui->plainTextEdit->toPlainText().toStdString();
	normalLength = QtUtils::readFloatFromQText(ui->lineEdit_normal_length->text(),
											   1.0f);
	drawNormal = ui->checkBox_draw_normals->isChecked();
	invertIntersection = ui->checkBox_invert->isChecked();

	convexGeometryColor.x = QtUtils::readFloatFromQText(ui->lineEdit_cg_r->text(),0.0f);
	convexGeometryColor.y = QtUtils::readFloatFromQText(ui->lineEdit_cg_g->text(),1.0f);
	convexGeometryColor.z = QtUtils::readFloatFromQText(ui->lineEdit_cg_b->text(),0.0f);
	convexGeometryColor.w = QtUtils::readFloatFromQText(ui->lineEdit_cg_a->text(),0.5f);

	convexGeometryNormalColor.x = QtUtils::readFloatFromQText(ui->lineEdit_cgn_r->text(),1.0f);
	convexGeometryNormalColor.y = QtUtils::readFloatFromQText(ui->lineEdit_cgn_g->text(),1.0f);
	convexGeometryNormalColor.z = QtUtils::readFloatFromQText(ui->lineEdit_cgn_b->text(),0.0f);
	convexGeometryNormalColor.w = QtUtils::readFloatFromQText(ui->lineEdit_cgn_a->text(),1.0f);

	borderCutColor.x = QtUtils::readFloatFromQText(ui->lineEdit_cbc_r->text(),1.0f);
	borderCutColor.y = QtUtils::readFloatFromQText(ui->lineEdit_cbc_g->text(),0.0f);
	borderCutColor.z = QtUtils::readFloatFromQText(ui->lineEdit_cbc_b->text(),0.0f);
	borderCutColor.w = QtUtils::readFloatFromQText(ui->lineEdit_cbc_a->text(),1.0f);

	outsideColor.x = QtUtils::readFloatFromQText(ui->lineEdit_outc_r->text(),1.0f);
	outsideColor.y = QtUtils::readFloatFromQText(ui->lineEdit_outc_g->text(),0.0f);
	outsideColor.z = QtUtils::readFloatFromQText(ui->lineEdit_outc_b->text(),0.0f);
	outsideColor.w = QtUtils::readFloatFromQText(ui->lineEdit_outc_a->text(),1.0f);

	outsideSelectedColor.x = QtUtils::readFloatFromQText(ui->lineEdit_outs_r->text(),1.0f);
	outsideSelectedColor.y = QtUtils::readFloatFromQText(ui->lineEdit_outs_g->text(),0.0f);
	outsideSelectedColor.z = QtUtils::readFloatFromQText(ui->lineEdit_outs_b->text(),0.0f);
	outsideSelectedColor.w = QtUtils::readFloatFromQText(ui->lineEdit_outs_a->text(),1.0f);

	drawOutsideElements = ui->checkBox_drawOutsideElements->isChecked();
	resetPosition = ui->radioButton__reset->isChecked();
}
void ConvexGeometryIntersectionRendererConfig::getFromFile(){
	QString filename = QFileDialog::getOpenFileName(this,
													("Open File"), "",
													"All Files(*);;");
	if(filename.size()==0)
		return;
	ui->plainTextEdit->setPlainText(QString::fromStdString(FileUtils::getFileToString(filename.toStdString())));
	textChangedChangeBool();
}
void ConvexGeometryIntersectionRendererConfig::saveToFile(){
	QString filename = QFileDialog::getSaveFileName(this,
													("Save File"), "",
													"All Files(*);;");
	if(filename.size()==0)
		return;
	FileUtils::saveToFile(filename.toStdString(),
						  ui->plainTextEdit->toPlainText().toStdString());
}
void ConvexGeometryIntersectionRendererConfig::textChangedChangeBool(){
	geometryTextChanged = true;
}
void ConvexGeometryIntersectionRendererConfig::changeTextToPolygonsFormat(){
	ui->plainTextEdit->setPlainText(QString("POLYGONS\n4\n0 0 0\n25 0 0\n0 25 0\n0 0 25\n4\n3 2 1 0\n3 1 2 3\n3 3 2 0\n3 3 0 1"));
	textChangedChangeBool();
}
void ConvexGeometryIntersectionRendererConfig::changeTextToNormalsFormat(){
	ui->plainTextEdit->setPlainText(QString("NORMALS\n3\n0 0 20 0 0 -1\n10 0 0 -1 0 0\n-10 0 0 1 0 0"));
	textChangedChangeBool();
}
void ConvexGeometryIntersectionRendererConfig::changeTextToSphereFormat(){
	ui->plainTextEdit->setPlainText(QString("SPHERE\n25.0\n25.0 0.0 0.0\n10000"));
	textChangedChangeBool();
}
void ConvexGeometryIntersectionRendererConfig::loadedModelIsLightWeight(bool isLightWeight){
	if(isLightWeight)
		ui->checkBox_move_with_model->setChecked(false);
	ui->checkBox_move_with_model->setEnabled(!isLightWeight);

}
