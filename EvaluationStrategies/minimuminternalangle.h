#ifndef MINIMUMINTERNALANGLE_H
#define MINIMUMINTERNALANGLE_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class MinimumInternalAngle: public EvaluationStrategy
{
	public:
		MinimumInternalAngle();
		virtual ~MinimumInternalAngle();
		virtual float value(vis::Polygon* m );
		virtual bool isFullFilled( vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MINIMUMINTERNALANGLE_H
