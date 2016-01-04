#include "GL/glew.h"
#include "visualizador.h"
#include "ui_visualizador.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QIODevice>
#include <QLabel>
#include <iostream>
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Model/Model.h"
#include "Model/Element/Element.h"
#include "Factories/EvaluationStrategyRegistry.h"
#include "Factories/selectionstrategyregistry.h"
#include "Factories/RendererRegistry.h"
#include "Factories/modelexportstrategyregistry.h"
#include "Factories/ModelLoadingFactory.h"
#include "Factories/PropertyFieldLoadingFactory.h"
#include "SelectionStrategies/Selection.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "SelectionStrategies/SelectionStrategyByProperty/selectionstrategybyproperty.h"
#include "SelectionStrategies/MouseSelection/mouseselection.h"
#include "ModelLoading/ModelLoadingStrategy.h"
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "ModelExport/modelexportstrategy.h"
#include "UI/CustomGLViewer.h"
#include "UI/rendererslist.h"
#include "Rendering/Renderer.h"
#include "Rendering/RModel/rmodel.h"
#include <unordered_map>
#include <QStandardItemModel>
#include "Utils/crosstimer.h"
#include "Utils/matrixtransformationutils.h"
#include "Utils/qtutils.h"
#include "Utils/lowmemoryhash.hpp"
#include "Utils/fileutils.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/triangle.h"
#include "Model/Element/Element.h"
Visualizador::Visualizador(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Visualizador),
	rendererConfigPopup(this),
	staticsFrame(this),
	selectionTableView(this),
	mainConfiguration(this),
	shortcutconfig(this),
	modelGeneralStaticsWidget(this),
	progressDialog(this),
	propertyFieldDialog(this)
{
	ui->setupUi(this);
	ui->dockWidget_evaluation_strategies->hide();
	evalStrategyConfigWidget = 0;
	renderersList = new RenderersList(ui->widget_list_renderer_cont);
	renderersList->show();
	ui->widget_list_renderer_cont->layout()->addWidget(renderersList);
	evaluationStrategyRegistry = EvaluationStrategyRegistry::getSingletonInstance();
	modelExportStrategyRegistry = ModelExportStrategyRegistry::getSingletonInstance();
	selectionStrategyRegistry = SelectionStrategyRegistry::getSingletonInstance();
	rmodel = new RModel();
	customGLViewer = new CustomGLViewer(rmodel,mainConfiguration,(*renderersList));
	customGLViewer->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	ui->centralVerticalLayout->addWidget(customGLViewer);
	selectionTableView.setSelection(&selection);
	customGLViewer->setSelection(&selection);
	mainConfiguration.readConfiguration();
	this->model = (Model*)0;
	//connect slots
	connect(ui->actionKey_Shortcuts,SIGNAL(triggered()),&shortcutconfig,SLOT(show()));
	connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(ui->actionOpen_Low_RAM_mode,SIGNAL(triggered()),this,SLOT(openFileLowWeight()));
	connect(ui->actionImport_property_field,SIGNAL(triggered()),this,SLOT(importFilePropertyField()));
	connect(renderersList,SIGNAL(secondaryRendererRemoved()),this,SLOT(secondaryRendererRemoved()));
	connect(ui->actionExport_complete,SIGNAL(triggered()),this,SLOT(exportModel()));
	connect(ui->actionSelection_As,SIGNAL(triggered()),this,SLOT(exportSelection()));
	connect(ui->pushButton_evaluate,SIGNAL( clicked()),this,SLOT(evaluate()));
	connect(ui->buttonSelect,SIGNAL( clicked()),this,SLOT(selectUsingSelectionStrategy()));
	connect(ui->comboRenderer,SIGNAL( currentIndexChanged(int)),this, SLOT(changeRenderer(int)));
	connect(ui->comboBox_selectStrategy,SIGNAL( currentIndexChanged(int)),this, SLOT(changeSelectionStrategy(int)));
	connect(this->ui->pushButton_show_renderer_config,SIGNAL(clicked()),&this->rendererConfigPopup, SLOT(show()));
	connect(this->ui->actionStatics,SIGNAL(triggered()),&this->staticsFrame, SLOT(refreshAndShow()));
	connect(this->ui->actionModel_General_Statics,
			SIGNAL(triggered()),
			this,
			SLOT(showModelGeneralStatics()));
	connect(this->ui->actionAbout,SIGNAL(triggered()),&this->aboutCamaron, SLOT(show()));
	connect(this->ui->actionPreferences,SIGNAL(triggered()),&this->mainConfiguration, SLOT(show()));
	connect(&this->mainConfiguration,SIGNAL(configurationUpdated()),this,SLOT(configurationUpdated()));
	connect(this->ui->actionSelection_Table, SIGNAL(triggered()), &this->selectionTableView,SLOT(show()));
	connect(this->ui->actionSelection_strategies,SIGNAL(triggered()),ui->dockWidget_selection_strategies,SLOT(show()));
	connect(this->ui->actionRenderers,SIGNAL(triggered()),ui->dockWidget_renderers,SLOT(show()));
	connect(this->ui->actionEvaluation_Strategies,SIGNAL(triggered()),ui->dockWidget_evaluation_strategies,SLOT(show()));
	connect(ui->actionClose_Model,SIGNAL(triggered()),this,SLOT(closeModel()));
	connect(&this->rendererConfigPopup, SIGNAL(applyChangesPushButton()),this->customGLViewer, SLOT(applyRendererConfigChanges()));
	connect(customGLViewer,SIGNAL(selectionBox(int,int,int,int)),this,SLOT(receiveSelectionBox(int,int,int,int)));
	connect(ui->pushButton_add_secondary,SIGNAL(clicked()),this,SLOT(addSecondaryRenderer()));
	connect(ui->pushButton_eval_strat_desc,SIGNAL(clicked()),this,SLOT(showEvaluationStrategyDescription()));
	connect(ui->comboBox_evalStrategies,SIGNAL(currentIndexChanged(int)),this,SLOT(changeEvaluationStrategy(int)));
	connect(ui->actionDetected_Video_Adapter,SIGNAL(triggered()),this,SLOT(glVersionPopup()));
	connect(ui->chkShowAxes,SIGNAL(toggled(bool)),this,SLOT(showAxesChanged(bool)));
	connect(&propertyFieldDialog,
			SIGNAL(onReadyToLoad(std::string, std::vector<std::shared_ptr<PropertyFieldDef> >)),
			this,
			SLOT(loadPropertyFieldsQThread(std::string, std::vector<std::shared_ptr<PropertyFieldDef> >)));
	modelLoadingFactory = ModelLoadingFactory::getSingletonInstance();
	modelLoadingFactoryLW = ModelLoadingFactory::getLightWeightSingletonInstance();
	propertyFieldLoadingFactory = PropertyFieldLoadingFactory::getSingletonInstance();
	fileFormats = getModelAcceptedExtensions(modelLoadingFactory);
	fileFormatsLW = getModelAcceptedExtensions(modelLoadingFactoryLW);
	fileFormatsPropertyField = getPropertyFieldAcceptedExtensions(propertyFieldLoadingFactory);
	fileExportFormats = getModelAcceptedExportExtensions();
	savedState = saveGeometry();
	modelGeneralStaticsCollected = false;
	dockList = findChildren<QDockWidget*>();
	std::cout << "Size of (Vis::Vertex)" << sizeof(vis::Vertex)<<std::endl;
	std::cout << "Size of (Vis::Triangle)" << sizeof(vis::Triangle)<<std::endl;
	std::cout << "Size of (Vis::Element)" << sizeof(vis::Element)<<std::endl;
	std::cout << "Size of (Vis::Polygon)" << sizeof(vis::Polygon)<<std::endl;
	std::cout << "Size of (Vis::Polyhedron)" << sizeof(vis::Polyhedron)<<std::endl;
	std::cout << "Size of (std::vector<vis::Vertex*>)" << sizeof(std::vector<vis::Vertex*>)<<std::endl;
	std::cout << "Size of (vis::Polyhedron*)" << sizeof(vis::Polyhedron*)<<std::endl;
	std::cout << "Size of (int)" << sizeof(int)<<std::endl;
	std::cout << "Size of (long)" << sizeof(long)<<std::endl;
	std::cout << "Size of (bool)" << sizeof(bool)<<std::endl;
	std::cout << "Size of (char)" << sizeof(char)<<std::endl;
	std::cout << "Size of (float)" << sizeof(float)<<std::endl;
	std::cout << "Size of (glm::vec3)" << sizeof(glm::vec3)<<std::endl;
	std::cout << "Size of (double)" << sizeof(double)<<std::endl;
	std::cout << "Size of (short)" << sizeof(short)<<std::endl;
	std::cout << "Size of (std::vector<int>)" << sizeof(std::vector<int>)<<std::endl;
	std::cout << "Size of (std::unordered_map<unsigned char, int>)" << sizeof(std::unordered_map<unsigned char, int>)<<std::endl;
	std::cout << "Size of (LowMemoryHash<unsigned char, int>)" << sizeof(LowMemoryHash<unsigned char, int>)<<std::endl;
	configurationUpdated();
	this->setWindowState(Qt::WindowMaximized);
	for (unsigned int i = 0; i < Visualizador::MAX_RECENT_FILES; ++i) {
		QAction* recentFileQAction = ui->menuRecent_files->addAction(QString::number(i));
		recentFilesQActions.push_back(recentFileQAction);
		recentFileQAction->setVisible(false);
		connect(recentFileQAction, SIGNAL(triggered()),
				this, SLOT(openRecentFile()));
	}
	ui->menuRecent_files->addSeparator();
	QAction* clearRecentFilesQAction = ui->menuRecent_files->addAction(QString("Clear files"));
	connect(clearRecentFilesQAction,SIGNAL(triggered()),this,SLOT(clearRecentFiles()));
	loadRecentFiles();
	connectModelLoadingStrategies(modelLoadingFactory);
	connectModelLoadingStrategies(modelLoadingFactoryLW);
	connectPropertyFieldLoadingStrategies(propertyFieldLoadingFactory);
	enableAndDisableWidgets();
}


Visualizador::~Visualizador()
{
	delete ui;
	delete rmodel;
	if(this->model)
		delete this->model;
	saveRecentFiles();
	//delete mouseSelection; selectionStrategyRegistry takes control of it

}

void Visualizador::loadRecentFiles(){
	std::string recentfilesFilename("recentFiles");
	recentFilesnames.clear();
	std::vector<std::string> rfilename;
	FileUtils::getFileToStringsByLine(recentfilesFilename,rfilename);
	recentFilesnames.insert(recentFilesnames.begin(),rfilename.begin(),
							rfilename.end());
	updateRecentFilesQActions();
}
void Visualizador::clearRecentFiles(){
	recentFilesnames.clear();
	updateRecentFilesQActions();
}

void Visualizador::updateRecentFilesQActions(){
	//hide All
	ui->menuRecent_files->setEnabled(false);
	for( QAction* action : recentFilesQActions){
		action->setVisible(false);
	}
	std::list<std::string>::iterator it;
	std::vector<QAction*>::size_type i = 0;
	for(it = recentFilesnames.begin();
		it!=recentFilesnames.end() && i < recentFilesQActions.size();it++){
		QAction* actionRecentFile = recentFilesQActions[i];
		actionRecentFile->setText(QString::fromStdString(
									  FileUtils::getFileNameWithoutPath(
										  *it)));
		actionRecentFile->setData(QString::fromStdString(*it));
		actionRecentFile->setVisible(true);
		++i;
	}
	if(i>0)
		ui->menuRecent_files->setEnabled(true);
}

void Visualizador::saveRecentFiles(){
	std::string filenames;
	std::list<std::string>::iterator it;
	for(it = recentFilesnames.begin();it!=recentFilesnames.end();it++)
		filenames += *it+"\n";
	FileUtils::saveToFile("recentFiles",filenames);
}

void Visualizador::addRecentFiles(QString filepath){
	//MAX_RECENT_FILES
	std::string filepathstd = filepath.toStdString();
	if(model&&model->isLightWeight())
		filepathstd = filepathstd + std::string("  -LW");
	recentFilesnames.push_front(filepathstd);
	std::list<std::string>::iterator it = recentFilesnames.begin();
	for(it++ ;it!=recentFilesnames.end();it++){
		if(!filepathstd.compare(*it)){
			//The filepath is twice in the list
			recentFilesnames.erase(it);
			break;
		}
	}
	while(recentFilesnames.size()>Visualizador::MAX_RECENT_FILES)
		recentFilesnames.pop_back();
	updateRecentFilesQActions();
}
void Visualizador::openRecentFile(){
	if(progressDialog.isVisible())
		return;
	QAction *action = qobject_cast<QAction *>(sender());
	if (action){
		QString data = action->data().toString();
		if(data.endsWith(QString("-LW"))){
			openModelFromFilePathQThread(data.left(data.length()-3).trimmed(),true);
		}
		else
			openModelFromFilePathQThread(data.trimmed(),false);
	}
}

void Visualizador::mouseDoubleClickEvent(QMouseEvent *e) {
	QWidget::mouseDoubleClickEvent(e);
	if(isFullScreen()) {
		this->showMaximized();
	} else {
		this->showFullScreen();
	}
}

std::string Visualizador::getModelAcceptedExtensions(ModelLoadingFactory* factory){
	std::string fileformats;
	std::vector<ModelLoadingStrategy*>& strategies = factory->getModelLoadingStrategies();
	for( ModelLoadingStrategy* strategy : strategies ) {
		std::vector<AcceptedFileFormat>& acceptedfileFormats = strategy->getFileFormats();
		for(AcceptedFileFormat& fileFormat : acceptedfileFormats ){
			fileformats += ";;"+fileFormat.fileFormatName+"(*."+
						   fileFormat.fileFormatExt+")";
		}
	}
	return "All Files(*)"+fileformats;
}
std::string Visualizador::getPropertyFieldAcceptedExtensions(PropertyFieldLoadingFactory* factory){
	std::string fileformats;
	std::vector<PropertyFieldLoadingStrategy*>& strategies = factory->getPropertyFieldLoadingStrategies();
	for(PropertyFieldLoadingStrategy* strategy : strategies ) {
		std::vector<AcceptedFileFormat>& acceptedfileFormats = strategy->getFileFormats();
		for( AcceptedFileFormat& fileFormat : acceptedfileFormats ){
			fileformats += ";;"+fileFormat.fileFormatName+"(*."+
						   fileFormat.fileFormatExt+")";
		}
	}
	return "All Files(*)"+fileformats;
}
std::string Visualizador::getModelAcceptedExportExtensions(){
	std::string fileformats;
	typedef std::map<std::string, ModelExportStrategy*>::iterator it_type;
	std::map<std::string,ModelExportStrategy*>* exportStrategies =
			modelExportStrategyRegistry->getRegistryMap();
	for(it_type iterator = exportStrategies->begin();
		iterator != exportStrategies->end(); iterator++) {
		ModelExportStrategy* p = iterator->second;

		fileformats += ";;"+p->getName()+"(*."+
					   p->getExtension()+")";
	}
	return "All Files(*)"+fileformats;
}
//Fill combobox with evaluation strategies
void Visualizador::fillEvaluationStrategiesComboBox(){
	typedef std::map<float,unsigned char>::iterator it_type;
	int npol = 0;
	for(it_type iterator = evaluationStrategyRegistry->getWeightMap()->begin(); iterator != evaluationStrategyRegistry->getWeightMap()->end(); iterator++) {
		int key = iterator->second;
		EvaluationStrategy* p = evaluationStrategyRegistry->getRegistryByKeyInstance(key);
		if(p){
			p->QApplicationInitiatedEv();
			this->ui->comboBox_evalStrategies->addItem(p->getName(), QVariant(key));
			if(p->hasQIcon())
				this->ui->comboBox_evalStrategies->setItemIcon(npol,*p->getEvaluationStrategyQIcon());
			npol++;
		}
	}
}

//Fill combobox with Selection strategies
void Visualizador::fillSelectionStrategiesComboBox(){
	typedef std::map<float,int>::iterator it_type;
	for(it_type iterator = selectionStrategyRegistry->getWeightMap()->begin();
		iterator != selectionStrategyRegistry->getWeightMap()->end(); iterator++) {
		int key = iterator->second;
		SelectionStrategy* p = selectionStrategyRegistry->getRegistryByKeyInstance(key);
		if(p){
			p->QApplicationInitiated();
			this->ui->comboBox_selectStrategy->addItem(p->getName(), QVariant(key));
		}
	}
}
//Fill combobox with Renderers
void Visualizador::fillRendererComboBox(){
	typedef std::map<float,int>::iterator it_type;
	ui->comboRenderer->clear();
	for(it_type iterator = rendererRegistry->getWeightMap()->begin();
		iterator != rendererRegistry->getWeightMap()->end(); iterator++) {
		int key = iterator->second;
		Renderer* p = rendererRegistry->getRegistryByKeyInstance(key);
		if(p->isOk()&&p->rmodelChanged(rmodel)){
			std::cout << "Renderer adding to combo: " << p->getRendererName() << std::endl;
			this->ui->comboRenderer->addItem(p->getRendererName(), QVariant(key));
		}
	}
}

void Visualizador::connectModelLoadingStrategies(ModelLoadingFactory *factory){
	for( ModelLoadingStrategy* strategy : factory->getModelLoadingStrategies() ) {
		connect(strategy,SIGNAL(modelLoadedSuccesfully()),
				this,SLOT(getLoadedModelFromLoadingStrategy()));
		connect(strategy,SIGNAL(setupProgressBarForNewModel(int,int,int,int)),
				&progressDialog,SLOT(setupProgressBarForNewModel(int,int,int,int)));
		connect(strategy,SIGNAL(setLoadedVertices(int)),
				&progressDialog,SLOT(setLoadedVertices(int)));
		connect(strategy,SIGNAL(setLoadedPolygons(int)),
				&progressDialog,SLOT(setLoadedPolygons(int)));
		connect(strategy,SIGNAL(setLoadedPolyhedrons(int)),
				&progressDialog,SLOT(setLoadedPolyhedrons(int)));
		connect(strategy,SIGNAL(stageComplete(int)),
				&progressDialog,SLOT(stageComplete(int)));
		connect(strategy,SIGNAL(addMessage(QString)),
				&progressDialog,SLOT(addMessage(QString)));
		connect(strategy,SIGNAL(errorLoadingModel(QString)),
				&progressDialog,SLOT(displayError(QString)));
		connect(strategy,SIGNAL(warningLoadingModel(QString)),
				&progressDialog,SLOT(displayWarning(QString)));
	}
}

void Visualizador::connectPropertyFieldLoadingStrategies(PropertyFieldLoadingFactory *factory){
	std::vector<PropertyFieldLoadingStrategy*>& loadingStrategies =
			factory->getPropertyFieldLoadingStrategies();

	for( PropertyFieldLoadingStrategy* strategy : loadingStrategies ) {
		connect(strategy,SIGNAL(propertyFieldsLoadedSuccesfully()),
				this,SLOT(onLoadedPropertyFields()));
		connect(strategy,SIGNAL(
					setupDialog(std::string,
						std::vector<std::shared_ptr<PropertyFieldDef>>)),
				&propertyFieldDialog,SLOT(setupForNewFile(std::string,
											  std::vector<std::shared_ptr<PropertyFieldDef>>)));
		connect(strategy,SIGNAL(setLoadedProgress(unsigned int)),
				&propertyFieldDialog,SLOT(setLoadedProgress(unsigned int)));
		connect(strategy,SIGNAL(addMessage(QString)),
				&propertyFieldDialog,SLOT(addMessage(QString)));
		connect(strategy,SIGNAL(errorLoadingPropertyField(QString)),
				&propertyFieldDialog,SLOT(displayError(QString)));
		connect(strategy,SIGNAL(warningLoadingPropertyField(QString)),
				&propertyFieldDialog,SLOT(displayWarning(QString)));
	}
}

void Visualizador::onLoadedPropertyFields() {
	std::cout << "Property fields loaded in: "<<timer.getTranscurredSeconds()<<std::endl;
	customGLViewer->forceReRendering();
	propertyFieldDialog.hide();
}

void Visualizador::exportModel(){
	if(!model)
		return;
	QString filename = QFileDialog::getSaveFileName(this,
													("Export Model"), "",
													fileExportFormats.c_str());
	std::cout << "EXPORT AS: " << filename.toStdString() << std::endl;

	if(filename.size()==0)
		return;
	try{
		if(!modelExportStrategyRegistry->exportModel(model,filename.toStdString())){
			QMessageBox::information(0,
									 QString("Export Error"),
									 QString("Failed to export the Model."), QMessageBox::Ok);
		}
	}catch(UnknownExtensionException& e){
		std::string message;
		message += "Extension not recognized to export<br>";
		message += "Filename: " +e.getFilename() +"<br>";
		message += "Extension: " + e.getExtension();
		QMessageBox::information(0,
								 QString("Model Export failed"),
								 QString(message.c_str()), QMessageBox::Ok);
	}catch(ExceptionMessage& e){

		QMessageBox::information(0,
								 QString("Model Export failed"),
								 QString(e.getMessage().c_str()), QMessageBox::Ok);
	}
}
void Visualizador::exportSelection(){
	if(!model)
		return;

	QString filename = QFileDialog::getSaveFileName(this,
													("Export Selection"), "",
													fileExportFormats.c_str());
	if(filename.size()==0)
		return;
	std::cout << "EXPORT Selection AS: " << filename.toStdString() << std::endl;
	try{if(!modelExportStrategyRegistry->exportSelection(&selection,filename.toStdString())){
			QMessageBox::information(0,
									 QString("Export Error"),
									 QString("Failed to export the Selection."), QMessageBox::Ok);
		}
	}catch(UnknownExtensionException& e){
		std::string message;
		message += "Extension not recognized to export<br>";
		message += "Filename: " +e.getFilename() +"<br>";
		message += "Extension: " + e.getExtension();
		QMessageBox::information(0,
								 QString("Selection Export failed"),
								 QString(message.c_str()), QMessageBox::Ok);
	}catch(ExceptionMessage& e){
		QMessageBox::information(0,
								 QString("Selection Export failed"),
								 QString(e.getMessage().c_str()), QMessageBox::Ok);
	}
}
void Visualizador::closeModel(){
	modelGeneralStaticsCollected = false;
	modelGeneralStaticsCollector.clean();
	modelGeneralStaticsWidget.clearData();
	if(model){
		rmodel->deleteData();
		this->selection.reset();
		this->selectionTableView.refreshSelectedElementsTable();
		this->customGLViewer->forceReRendering();
		delete model;
		model = (Model*)0;
		typedef std::map<unsigned char, EvaluationStrategy*>::iterator it_type;
		for(it_type iterator = evaluationStrategyRegistry->getRegistryMap()->begin(); iterator != evaluationStrategyRegistry->getRegistryMap()->end(); iterator++) {
			EvaluationStrategy* p = iterator->second;
			p->setTotalElementsCount(0);
		}
		fillRendererComboBox();
		renderersList->removeAllRenderers();
	}
	enableAndDisableWidgets();
}

void Visualizador::openFile()
{
	if(progressDialog.isVisible())
		return;
	QString filename = QFileDialog::getOpenFileName(this,
													("Open File"), "",
													fileFormats.c_str());
	if(filename.size()==0)
		return;
	openModelFromFilePathQThread(filename,false);

}
void Visualizador::openFileLowWeight()
{
	if(progressDialog.isVisible())
		return;
	QString filename = QFileDialog::getOpenFileName(this,
													("Open File (Low RAM)"), "",
													fileFormatsLW.c_str());
	if(filename.size()==0)
		return;
	openModelFromFilePathQThread(filename,true);

}
void Visualizador::importFilePropertyField() {
	if(progressDialog.isVisible())
		return;
	QString filename = QFileDialog::getOpenFileName(this,
													("Open Scalar Field File"), "",
													fileFormatsLW.c_str());
	if(filename.size()==0)
		return;
	openPropertyFieldDialogFromFilePath(filename);
}

void Visualizador::openModelFromFilePath(QString filename, bool addToRecentFiles){
	CrossTimer crossTimer;
	CrossTimer crossTimerModel;
	try{
		closeModel();
		Model* loaded = modelLoadingFactory->loadModel(filename.toStdString());
		std::cout << "Visualizador::openFile() Creating Base Model: " << crossTimerModel.getTranscurredSeconds() << " seconds" << std::endl;
		if( loaded ) {
			this->selection.reset();
			this->model = loaded;
			setupEvaluationStrategiesStatics();
			this->customGLViewer->resetCameraPosition();
			this->customGLViewer->refreshHelpers();
			this->selectionTableView.refreshSelectedElementsTable();
			this->model->loadModelDataIntoRModel(this->rmodel);
			this->selection.setRModel(rmodel);
			this->fillRendererComboBox();
			if(modelGeneralStaticsWidget.isVisible()){
				model->fillModelStatsDD(&modelGeneralStaticsCollector);
				modelGeneralStaticsWidget.setData(&modelGeneralStaticsCollector);
				modelGeneralStaticsCollected = true;
			}
			if(addToRecentFiles)
				addRecentFiles(filename);
			this->customGLViewer->forceReRendering();

		}
		std::cout << "Visualizador::OpenFile() Complete Model: " << crossTimer.getTranscurredSeconds() << " seconds" << std::endl;
	}
	catch(ExceptionMessage& e){

		QMessageBox::information(0,
								 QString("Failed to open Model"),
								 QString(e.getMessage().c_str()), QMessageBox::Ok);
	}catch(UnknownExtensionException& e){
		std::string message;
		message += "Extension not recognized to load model<br>";
		message += "Filename: " +e.getFilename() +"<br>";
		message += "Extension: " + e.getExtension();
		QMessageBox::information(0,
								 QString("Model loading failed"),
								 QString(message.c_str()), QMessageBox::Ok);
	}catch(std::bad_alloc &ba){
		closeModel();
		QMessageBox::information(0,
								 QString("Model Loading Failed"),
								 QString("Not enough RAM to load the model, try the x64 version"),QMessageBox::Ok);
	}

}
void Visualizador::openModelFromFilePathQThread(QString filename,bool lw){
	try{
		this->customGLViewer->refreshHelpers();
		progressDialog.setModelName(FileUtils::getFileNameWithoutPath(filename.toStdString()));
		progressDialog.setupForNewModel(vis::CONSTANTS::VERTEX_CLOUD,0);
		progressDialog.show();
		closeModel();
		ModelLoadingStrategy* selectedStrategy;
		ModelLoadingFactory* selectedFactory = lw?modelLoadingFactoryLW:
												  modelLoadingFactory;
		selectedStrategy = selectedFactory->loadModelQThread(filename.toStdString());
		selectedStrategy->loadModelQThread(filename.toStdString());
		timer = CrossTimer();
	}
	catch(ExceptionMessage& e){
		progressDialog.close();
		QMessageBox::information(0,
								 QString("Failed to open Model"),
								 QString(e.getMessage().c_str()), QMessageBox::Ok);
	}catch(UnknownExtensionException& e){
		progressDialog.close();
		std::string message;
		message += "Extension not recognized to load model<br>";
		message += "Filename: " +e.getFilename() +"<br>";
		message += "Extension: " + e.getExtension();
		QMessageBox::information(0,
								 QString("Model loading failed"),
								 QString(message.c_str()), QMessageBox::Ok);
	}catch(std::bad_alloc &ba){
		progressDialog.close();
		closeModel();
		QMessageBox::information(0,
								 QString("Model Loading Failed"),
								 QString("Not enough RAM to load the model, try the x64 version"),QMessageBox::Ok);
	}
}

void Visualizador::openPropertyFieldDialogFromFilePath(QString filename){
	try{
		PropertyFieldLoadingStrategy* selectedStrategy;
		selectedStrategy = propertyFieldLoadingFactory->loadPropertyFieldQThread(filename.toStdString());
		std::vector<std::shared_ptr<PropertyFieldDef>> props = selectedStrategy->loadDefs(filename.toStdString());
		propertyFieldDialog.setupForPropertyFields(filename.toStdString(), props);
		propertyFieldDialog.show();
	}
	catch(ExceptionMessage& e){
		propertyFieldDialog.close();
		QMessageBox::information(0,
								 QString("Failed to open property field"),
								 QString(e.getMessage().c_str()), QMessageBox::Ok);
	}catch(UnknownExtensionException& e){
		propertyFieldDialog.close();
		std::string message;
		message += "Extension not recognized to load property field<br>";
		message += "Filename: " +e.getFilename() +"<br>";
		message += "Extension: " + e.getExtension();
		QMessageBox::information(0,
								 QString("Property field loading failed"),
								 QString(message.c_str()), QMessageBox::Ok);
	}catch(std::bad_alloc &ba){
		propertyFieldDialog.close();
		closeModel();
		QMessageBox::information(0,
								 QString("Property field loading Failed"),
								 QString("Not enough RAM to load the model, try the x64 version"),QMessageBox::Ok);
	}
}

void Visualizador::loadPropertyFieldsQThread(std::string filename,std::vector<std::shared_ptr<PropertyFieldDef>> props) {
	std::cout << "Loading property field " << filename;
	try{
		//this->customGLViewer->refreshHelpers();
		//propertyFieldDialog.setPropertyFieldName(.setModelName(FileUtils::getFileNameWithoutPath(filename.toStdString()));
		PropertyFieldLoadingStrategy* selectedStrategy;
		selectedStrategy = propertyFieldLoadingFactory->loadPropertyFieldQThread(filename);
		timer = CrossTimer();
		selectedStrategy->loadPropertyFieldQThread(filename,model,props);
	}
	catch(ExceptionMessage& e){
		propertyFieldDialog.close();
		QMessageBox::information(0,
								 QString("Failed to open Model"),
								 QString(e.getMessage().c_str()), QMessageBox::Ok);
	}catch(UnknownExtensionException& e){
		propertyFieldDialog.close();
		std::string message;
		message += "Extension not recognized to load model<br>";
		message += "Filename: " +e.getFilename() +"<br>";
		message += "Extension: " + e.getExtension();
		QMessageBox::information(0,
								 QString("Model loading failed"),
								 QString(message.c_str()), QMessageBox::Ok);
	}catch(std::bad_alloc &ba){
		propertyFieldDialog.close();
		closeModel();
		QMessageBox::information(0,
								 QString("Model Loading Failed"),
								 QString("Not enough RAM to load the model, try the x64 version"),QMessageBox::Ok);
	}
}

void Visualizador::getLoadedModelFromLoadingStrategy(){
	ModelLoadingStrategy *loadingstrategy;
	loadingstrategy = qobject_cast<ModelLoadingStrategy *>(sender());
	Model* loaded = 0;
	if (loadingstrategy)
		loaded = loadingstrategy->getLoadedModel();
	if( loaded ) {
		this->selection.reset();
		this->model = loaded;
		setupEvaluationStrategiesStatics();
		this->customGLViewer->resetCameraPosition();
		this->customGLViewer->refreshHelpers();
		this->selectionTableView.refreshSelectedElementsTable();
		enableAndDisableWidgets();
		progressDialog.stageComplete(ModelLoadingProgressDialog::CAMARON_ELEMENTS_CONFIGURATED);
		this->model->loadModelDataIntoRModel(this->rmodel);
		progressDialog.stageComplete(ModelLoadingProgressDialog::RMODEL_READY);
		this->selection.setRModel(rmodel);
		this->fillRendererComboBox();
		if(modelGeneralStaticsWidget.isVisible() || loaded->isLightWeight()){
			model->fillModelStatsDD(&modelGeneralStaticsCollector);
			modelGeneralStaticsWidget.setData(&modelGeneralStaticsCollector);
			modelGeneralStaticsCollected = true;
		}
		std::cout << "Model loaded in: "<<timer.getTranscurredSeconds()<<std::endl;
		addRecentFiles(QString::fromStdString(loaded->getFilename()));
		loaded->clean();//will clear unused data for LightWeight models
		this->customGLViewer->forceReRendering();
	}
	progressDialog.hide();
}
void Visualizador::enableAndDisableWidgets(){
	ui->actionClose_Model->setEnabled(model != 0);
	ui->menuExport_As->setEnabled(model != 0);
	ui->actionImport_property_field->setEnabled(model != 0);
	if(!model)
		return;
	ui->dockWidget_evaluation_strategies->setDisabled(model->isLightWeight());
	ui->dockWidget_selection_strategies->setDisabled(model->isLightWeight());
	ui->actionSelection_Table->setDisabled(model->isLightWeight());
	ui->menuExport_As->setDisabled(model->isLightWeight());
	ui->actionStatics->setDisabled(model->isLightWeight());

}

void Visualizador::setupEvaluationStrategiesStatics(){
	typedef std::map<unsigned char, EvaluationStrategy*>::iterator it_type;
	for(it_type iterator = evaluationStrategyRegistry->getRegistryMap()->begin(); iterator != evaluationStrategyRegistry->getRegistryMap()->end(); iterator++) {
		EvaluationStrategy* p = iterator->second;
		this->model->setupEvaluationStrategy(p);
	}
}

void Visualizador::evaluate()
{
	//	Qt::CheckState checked = this->ui->checkBox_evalOnSelection->checkState();
	int index = this->ui->comboBox_evalStrategies->currentIndex();
	index = this->ui->comboBox_evalStrategies->itemData(index).toInt();
	bool applyOnSelection = ui->checkBox_evaluate_selection->isChecked();
	if(!this->model){
		QMessageBox::information(0,
								 QString("Evaluation Failed"),
								 QString("You must load a Model to evaluate it."), QMessageBox::Ok);
		return;
	}
	EvaluationStrategy* strategy = this->evaluationStrategyRegistry->getRegistryByKeyInstance(index);
	if(!strategy){
		std::cerr << "Trying to apply invalid evaluation strategy, key: " << index << std::endl;
		return;
	}
	if(applyOnSelection){
#ifdef DEBUG_MOD
		std::cout << "Applying Strategy " << strategy->getName() << " on Selection." << std::endl;
#endif
		return;
	}
#ifdef DEBUG_MOD
	std::cout << "Applying Strategy " << strategy->getName() << " on the model." << std::endl;
#endif
	this->model->evaluateUsingEvaluationStrategy(strategy);
}


void Visualizador::showModelGeneralStatics(){
	if(!modelGeneralStaticsCollected && model){
		model->fillModelStatsDD(&modelGeneralStaticsCollector);
		modelGeneralStaticsWidget.setData(&modelGeneralStaticsCollector);
		modelGeneralStaticsCollected = true;
	}
	modelGeneralStaticsWidget.show();
}

void Visualizador::selectUsingSelectionStrategy(){
	if(!this->model)
		return;
	CrossTimer crossTimer;
	if(!this->selectionStrategy->setup())
		return;
	if(this->selectionStrategy->isAppliedOnSelection()){
		if(!this->selectionStrategy->setupPreApplying(&selection,rmodel))
			return;
		this->selectionStrategy->selectElement(&selection);
		this->selectionTableView.refreshSelectedElementsTable();
		this->rmodel->refreshSelectedElements();
		this->customGLViewer->forceReRendering();
	}
	else if(this->model->selectUsingSelectionStrategy(this->selectionStrategy,rmodel, &selection)){
		this->selectionTableView.refreshSelectedElementsTable();
		this->rmodel->refreshSelectedElements();
		this->customGLViewer->forceReRendering();
	}
	std::cout << "Visualizador::selectUsingSelectionStrategy(): " << crossTimer.getTranscurredSeconds() << " seconds" << std::endl;
}
void Visualizador::glVersionPopup(){
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	QString glvendor = QString("GL Vendor : ")+QString((const char*)vendor);
	QString glrenderer = QString("\nGL Renderer : ")+QString((const char*)renderer);
	QString glversions = QString("\nGL Version (string) : ")+QString((const char*)version);
	QString glversioni = QString("\nGL Version (integer) : ")+
						 QString::number(major)+QString(".")+QString::number(minor);
	QString glslversion = QString("\nGLSL Version : ")+QString((const char*)glslVersion);
	QMessageBox s;
	s.setText(glvendor+glrenderer+glversions+glversioni+glslversion);
	s.exec();
}
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrenderer.h"
bool Visualizador::initGlewAndRendererFactory(){
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		return false;
	}
	this->fillEvaluationStrategiesComboBox();//first
	rendererRegistry = RendererRegistry::getSingletonInstance();
	this->rendererRegistry->glewIsReady();
	mouseSelection = new MouseSelection();
	selectionStrategyRegistry->insertNextSelectionStrategy(mouseSelection);
	//this->changeRenderer(0);
	staticsFrame.fillEvaluationStrategiesComboBox();
	this->fillSelectionStrategiesComboBox();
	this->fillRendererComboBox();
	customGLViewer->glewInitiated();
	return true;
}

void Visualizador::addSecondaryRenderer(){
	Renderer* rende = customGLViewer->getRenderer();
	if(rende && rende->isValidAsSecondaryRenderer())
		renderersList->addRenderer(customGLViewer->getRenderer());
}
void Visualizador::secondaryRendererRemoved(){
	this->customGLViewer->forceReRendering();
}

void Visualizador::changeRenderer(int index){
	int key = this->ui->comboRenderer->itemData(index).toInt();
	this->rendererConfigPopup.hide();
	this->rendererConfigPopup.cleanConfigQWidget();
	Renderer* selectedRenderer = this->rendererRegistry->getRegistryByKeyInstance(key);

	this->ui->pushButton_show_renderer_config->setDisabled(true);
	if(selectedRenderer->hasRendererConfigWidget()){
		this->ui->pushButton_show_renderer_config->setDisabled(false);
		this->rendererConfigPopup.setConfigQWidget(selectedRenderer->getRendererConfigWidget());
	}
	//ui->widgetRendererConfig->layout()->addWidget(selectedRenderer->getConfigWidget());
	std::cout << "Selected Renderer: " << selectedRenderer->getRendererName() << std::endl;
	this->customGLViewer->setRenderer(selectedRenderer);
	this->customGLViewer->forceReRendering();
}
void Visualizador::changeSelectionStrategy(int index){
	int key = this->ui->comboBox_selectStrategy->itemData(index).toInt();
	this->ui->widget_selectionStrategyConfigContainer->hide();
	//Remove current config widget
	cleanSelectionStrategyConfigQWidget();
	selectionStrategy = this->selectionStrategyRegistry->getRegistryByKeyInstance(key);
	//this->ui->pushButton_show_renderer_config->setDisabled(true);
	if(selectionStrategy->hasConfigWidget()){
		QWidget* configWid = selectionStrategy->getConfigWidget();

		this->ui->widget_selectionStrategyConfigContainer->layout()->addWidget(configWid);
		//ui->selectStrategiesDockWidget->setMaximumHeight(configWid->height());
		this->ui->widget_selectionStrategyConfigContainer->show();
		configWid->show();
	}
	QtUtils::fixDockingWidgetSize(ui->dockWidget_selection_strategies,
								  ui->widget_selectionStrategyConfigContainer);
	fixDockWidgetPositions();
	ui->buttonSelect->setVisible(selectionStrategy->applyButtonEnabled());
}

void Visualizador::cleanSelectionStrategyConfigQWidget(){
	if(!ui->widget_selectionStrategyConfigContainer->layout()->isEmpty()){
		QLayoutItem* item;
		while ( (item = ui->widget_selectionStrategyConfigContainer->layout()->takeAt(0)) != 0){
			QWidget * widget = item->widget();
			if(widget!=0)
				widget->hide();
		}
	}
}
void Visualizador::changeEvaluationStrategy(int index){
	int key = this->ui->comboBox_evalStrategies->itemData(index).toInt();
	if(this->evalStrategyConfigWidget)
		this->evalStrategyConfigWidget->hide();
	//Remove current config widget
	ui->pushButton_eval_strat_desc->setEnabled(false);
	EvaluationStrategy* evalStrategy = this->evaluationStrategyRegistry->getRegistryByKeyInstance(key);
	if(!evalStrategy)
		return;
	ui->pushButton_eval_strat_desc->setToolTip(QString::fromStdString(evalStrategy->getDescription()));
	if(evalStrategy->hasDescriptionQWidget()){
		ui->pushButton_eval_strat_desc->setEnabled(true);
		evalStrategyConfigWidget = evalStrategy->getEvaluationStrategyDescriptionQWidget();
	}
}
void Visualizador::showEvaluationStrategyDescription(){
	if(evalStrategyConfigWidget)
		evalStrategyConfigWidget->show();
}

void Visualizador::configurationUpdated(){
	rmodel->setPerspectiveValue(mainConfiguration.perspectiveValue);
	rmodel->setUseOrthoProjection(mainConfiguration.useOrthoProjection);
	rmodel->setBackgroundColor(mainConfiguration.backgroundColor);
	customGLViewer->forceReRendering();
}

void Visualizador::receiveSelectionBox(int x1,int y1,int x2,int y2){
	if(!model||model->isLightWeight())
		return;
	if(model){
		mouseSelection->setUseOrthoProjection(rmodel->isOrthoProjectionInUse());
		mouseSelection->setMVPVP(customGLViewer->getRenderer()->getMVPVP(rmodel));
		mouseSelection->setMVP(customGLViewer->getRenderer()->getMVP(rmodel));
		mouseSelection->setVP(rmodel->getViewPortMatrix());
		mouseSelection->setNormalMatrix(glm::mat3(customGLViewer->getRenderer()->getMV(rmodel)));
	}
	if(x1 == x2 && y1 == y2)
		mouseSelection->setPointSelection(glm::vec2(x1,y1));
	else
		mouseSelection->setRectCorners(glm::vec2(x1,y1),glm::vec2(x2,y2));
	mouseSelection->setViewPortSize(glm::vec2(customGLViewer->width(),customGLViewer->height()));
	SelectionStrategy* temp = selectionStrategy;
	selectionStrategy = mouseSelection;
	selectUsingSelectionStrategy();
	selectionStrategy = temp;
}
void Visualizador::fixDockWidgetPositions(){
	std::map<int,QDockWidget*> ordered;
	for (int i = 0; i < dockList.count(); i++) {
		QDockWidget *dock = dockList.at(i);
		if(!dock->isVisible() || dock->isFloating())
			continue;
		int dockTopLeft = dock->geometry().topLeft().y();
		ordered.insert(std::make_pair(dockTopLeft,dock));
		std::cout<<"Y:: " << dockTopLeft;
	}
	int accum = 0;
	typedef std::map<int,QDockWidget*>::iterator it_type;
	for(it_type i = ordered.begin();i!=ordered.end();i++){
		QDockWidget *dock = i->second;
		dock->move(QPoint(dock->geometry().topLeft().x(),accum));
		accum+=i->first;
	}
}

void Visualizador::showAxesChanged(bool value){
	this->customGLViewer->setAxesVisible(value);
	this->customGLViewer->forceReRendering();
}
