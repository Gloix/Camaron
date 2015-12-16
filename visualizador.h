#ifndef VISUALIZADOR_H
#define VISUALIZADOR_H

#define TAB_SELECT_USING_GEOMETRY_INDEX 0
#define TAB_SELECT_USING_EVALUATION_STRATEGY_INDEX 1
#include <list>
#include <QMainWindow>
#include <QWidget>
#include "rendererconfigpopup.h"
#include "SelectionStrategies/Selection.h"
#include "UI/staticsframe.h"
#include "UI/selectiontableview.h"
#include "UI/mainconfiguration.h"
#include "UI/modelgeneralstaticswidget.h"
#include "UI/aboutcamaron.h"
#include "UI/KeyshortcutConfig/keyshortcutconfiguration.h"
#include "ModelGeneralStatics/modelgeneralstaticscollector.h"
#include "UI/modelloadingprogressdialog.h"
#include "UI/propertyfieldloaddialog.h"
#include "Utils/crosstimer.h"
class CustomGLViewer;
namespace Ui {
	class Visualizador;
}
class EvaluationStrategyRegistry;
class ModelExportStrategyRegistry;
class SelectionStrategyRegistry;
class RendererRegistry;
class SelectionStrategy;
class ModelLoadingFactory;
class PropertyFieldLoadingFactory;
class RModel;
class RenderersList;
class Model;
class MouseSelection;
struct SelectionStrategyData;
class Visualizador : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit Visualizador(QWidget *parent = 0);
		~Visualizador();
		bool initGlewAndRendererFactory();
	public slots:
		void closeModel();
		void openFile();
		void openFileLowWeight();
		void importFileScalarField();
		void exportModel();
		void exportSelection();
		void selectUsingSelectionStrategy();
		void evaluate();
		void changeRenderer(int);
		void changeSelectionStrategy(int);
		void changeEvaluationStrategy(int);
		void showEvaluationStrategyDescription();
		void configurationUpdated();
		void receiveSelectionBox(int,int,int,int);
		void showModelGeneralStatics();
		void addSecondaryRenderer();
		void secondaryRendererRemoved();
		void mouseDoubleClickEvent(QMouseEvent *e);
		void openRecentFile();
		void glVersionPopup();
		void clearRecentFiles();
		void getLoadedModelFromLoadingStrategy();
		void showAxesChanged(bool value);
	private:
		static const unsigned int MAX_RECENT_FILES = 5;
		void fixDockWidgetPositions();
		void cleanSelectionStrategyConfigQWidget();
		void setBaseSelectionStrategyData(SelectionStrategyData*);
		void fillEvaluationStrategiesComboBox();
		void fillSelectionStrategiesComboBox();
		void fillRendererComboBox();
		void connectModelLoadingStrategies(ModelLoadingFactory*);
		void connectPropertyFieldLoadingStrategies(PropertyFieldLoadingFactory*);
		void loadRecentFiles();
		void saveRecentFiles();
		void updateRecentFilesQActions();
		void addRecentFiles(QString filepath);
		void enableAndDisableWidgets();

		std::string getModelAcceptedExtensions(ModelLoadingFactory* factory);
		std::string getPropertyFieldAcceptedExtensions(PropertyFieldLoadingFactory* factory);
		std::string getModelAcceptedExportExtensions();
		void setupEvaluationStrategiesStatics();
		void openModelFromFilePath(QString, bool addToRecentFiles);
		void openModelFromFilePathQThread(QString filename,bool lw);
		Ui::Visualizador *ui;

		void onLoadedPropertyFields();

		//Model
		bool modelGeneralStaticsCollected;
		Selection selection;
		Model* model;
		ModelGeneralStaticsCollector modelGeneralStaticsCollector;
		//Model Loading
		std::string fileFormats,fileFormatsLW, fileFormatsPropertyField;
		std::string fileExportFormats;
		//Model Rendering
		CustomGLViewer* customGLViewer;
		RenderersList* renderersList;
		RendererConfigPopUp rendererConfigPopup;
		RModel* rmodel;

		//SelectionStrategy
		SelectionStrategy* selectionStrategy;
		MouseSelection* mouseSelection;
		//Registries
		RendererRegistry* rendererRegistry;
		EvaluationStrategyRegistry* evaluationStrategyRegistry;
		ModelLoadingFactory* modelLoadingFactory;
		ModelLoadingFactory* modelLoadingFactoryLW;
		PropertyFieldLoadingFactory* propertyFieldLoadingFactory;
		SelectionStrategyRegistry* selectionStrategyRegistry;
		ModelExportStrategyRegistry* modelExportStrategyRegistry;

		//Statistics
		StaticsFrame staticsFrame;

		//Selection Table
		SelectionTableView selectionTableView;
		MainConfiguration mainConfiguration;
		KeyShortcutConfiguration shortcutconfig;
		ModelGeneralStaticsWidget modelGeneralStaticsWidget;

		//State
		QByteArray savedState;
		//QDockingWidgetOrder
		QList<QDockWidget*> dockList;
		//Camaron About
		AboutCamaron aboutCamaron;
		//Evaluation Strategy Config
		QWidget* evalStrategyConfigWidget;
		std::vector<QAction*> recentFilesQActions;
		std::list<std::string> recentFilesnames;

		ModelLoadingProgressDialog progressDialog;
		PropertyFieldLoadDialog propertyFieldDialog;
		CrossTimer timer;
};

#endif // VISUALIZADOR_H
