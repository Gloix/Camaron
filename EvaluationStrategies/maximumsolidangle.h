#ifndef MAXIMUMSOLIDANGLE_H
#define MAXIMUMSOLIDANGLE_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class MaximumSolidAngle: public EvaluationStrategy
{
	public:
		MaximumSolidAngle();
		virtual ~MaximumSolidAngle();
		virtual float value(vis::Polyhedron* m );
		virtual bool isFullFilled( vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MAXIMUMSOLIDANGLE_H
