#ifndef TRIANGLERADIUSRATIO_H
#define TRIANGLERADIUSRATIO_H
#include "EvaluationStrategies/EvaluationStrategy.h"
class TriangleRadiusRatio:public EvaluationStrategy
{
	public:
		TriangleRadiusRatio();
		virtual ~TriangleRadiusRatio();
		float value( vis::Triangle* t);
		bool isFullFilled( vis::Triangle* t);
		float getNullValue();
		bool hasDescriptionQWidget();
		QWidget* getEvaluationStrategyDescriptionQWidget();
		void QApplicationInitiatedEv();
	private:
		QWidget* descriptionWidget;
};

#endif // TRIANGLERADIUSRATIO_H
