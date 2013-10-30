#ifndef MAINCONFIGURATION_H
#define MAINCONFIGURATION_H

#include <QWidget>
#include <glm/glm.hpp>
namespace Ui {
class MainConfiguration;
}
class GenericConfigWidget;
class MainConfiguration : public QWidget
{
		Q_OBJECT
		
	public:
		explicit MainConfiguration(QWidget *parent = 0);
		~MainConfiguration();
		void addNewTabWidget(GenericConfigWidget*);
		void weightedAverageFail();

		//Render Basics
		glm::vec4 backgroundColor;
		glm::vec4 frameRateFontColor;
		glm::vec4 frameRateFontBoxColor;
		int frameFontSize;
		bool displayFrameRate;
		bool useOrthoProjection;
		float perspectiveValue;

		//Opengl Properties
		float pointSize;
		float linewidth;
		float antialiasingEnabled;
		bool transparencyEnabled;
		bool transparency2passEnabled;
		int oitTechnique;
		static const int USE_WEIGHTED_AVERAGE = 2;
		static const int USE_NONE = 0;
		static const int USE_WEIGHTED_SUM = 1;
		float alphatest_value;
		bool faceCullingEnabled;
		bool backFaceIsCCW;
	public slots:
		void readConfiguration();
	signals:
		void configurationUpdated();
	private:
		Ui::MainConfiguration *ui;
};

#endif // MAINCONFIGURATION_H
