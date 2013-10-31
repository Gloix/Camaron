#ifndef MAXIMUMINTERNALANGLE_H
#define MAXIMUMINTERNALANGLE_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class MaximumInternalAngle: public EvaluationStrategy
{
	public:
		MaximumInternalAngle();
		virtual ~MaximumInternalAngle();
		virtual float value(vis::Polygon* m );
		virtual bool isFullFilled( vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};
#endif // MAXIMUMINTERNALANGLE_H
