#ifndef PROPERTYVALUESRENDERERCONFIG_H
#define PROPERTYVALUESRENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
namespace Ui {
class PropertyValuesRendererConfig;
}
class EvaluationStrategyRegistry;
class EvaluationStrategy;
class PropertyValuesRendererConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit PropertyValuesRendererConfig(QWidget *parent = 0);
		~PropertyValuesRendererConfig();
		void readConfig();
		//Config Properties
		EvaluationStrategy* evaluationStrategy;
		glm::vec4 minValueColor;
		glm::vec4 maxValueColor;
		glm::vec4 noValueColor;
		glm::vec4 wireFrameColors;
		glm::vec4 wireFrameSelectedColors;
		int wireFrameLineWidth;
		int wireFrameSelectedLineWidth;
		bool drawWireFrame;
		bool changedEvaluationStrategy;
		bool drawOnlySelectedElements;
	private:
		Ui::PropertyValuesRendererConfig *ui;
		EvaluationStrategyRegistry* evaluationStrategyRegistry;
		void fillEvaluationStrategiesComboBox();
};

#endif // PROPERTYVALUESRENDERERCONFIG_H
