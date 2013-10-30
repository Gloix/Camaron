#ifndef MAXIMUMDIHEDRALANGLE_H
#define MAXIMUMDIHEDRALANGLE_H
#include "EvaluationStrategies/EvaluationStrategy.h"
class MaximumDihedralAngle: public EvaluationStrategy
{
	public:
		MaximumDihedralAngle();
		virtual ~MaximumDihedralAngle();
		virtual float value(vis::Polygon* m );
		virtual bool isFullFilled( vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MAXIMUMDIHEDRALANGLE_H
