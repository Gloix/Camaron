#ifndef TRIANGLEQUALITYMEASURE1_H
#define TRIANGLEQUALITYMEASURE1_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class TriangleQualityMeasure1:public EvaluationStrategy
{
	public:
		TriangleQualityMeasure1();
		virtual float value(vis::Triangle* m );
		virtual bool isFullFilled( vis::Triangle* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // TRIANGLEQUALITYMEASURE1_H
