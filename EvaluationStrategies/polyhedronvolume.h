#ifndef POLYHEDRONVOLUME_H
#define POLYHEDRONVOLUME_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class PolyhedronVolume: public EvaluationStrategy
{
	public:
		PolyhedronVolume();
		~PolyhedronVolume();
		virtual float value( vis::Polyhedron* m );
		virtual bool isFullFilled( vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // POLYHEDRONVOLUME_H
