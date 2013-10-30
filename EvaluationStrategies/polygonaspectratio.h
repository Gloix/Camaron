#ifndef POLYGONASPECTRATIO_H
#define POLYGONASPECTRATIO_H

#include "EvaluationStrategies/EvaluationStrategy.h"

class PolygonAspectRatio: public EvaluationStrategy
{
	public:
		PolygonAspectRatio();
		virtual ~PolygonAspectRatio();
		virtual float value(vis::Polygon* m );
		virtual bool isFullFilled(vis::Polygon* m );
		virtual float value(vis::Triangle* m );
		virtual bool isFullFilled(vis::Triangle* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // POLYGONASPECTRATIO_H
