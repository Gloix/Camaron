#ifndef MINIMUMSOLIDANGLE_H
#define MINIMUMSOLIDANGLE_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class MinimumSolidAngle: public EvaluationStrategy
{
	public:
		MinimumSolidAngle();
		virtual ~MinimumSolidAngle();
		virtual float value(vis::Polyhedron* m );
		virtual bool isFullFilled( vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MINIMUMSOLIDANGLE_H
