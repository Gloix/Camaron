#ifndef MINIMUMDIHEDRALANGLE_H
#define MINIMUMDIHEDRALANGLE_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class MinimumDihedralAngle: public EvaluationStrategy
{
	public:
		MinimumDihedralAngle();
		virtual ~MinimumDihedralAngle();
		virtual float value(vis::Polygon* m );
		virtual bool isFullFilled( vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MINIMUMDIHEDRALANGLE_H
