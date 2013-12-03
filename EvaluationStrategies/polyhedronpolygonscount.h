#ifndef POLYHEDRONPOLYGONSCOUNT_H
#define POLYHEDRONPOLYGONSCOUNT_H

#include "EvaluationStrategies/EvaluationStrategy.h"

class PolyhedronPolygonsCount: public EvaluationStrategy
{
	public:
		PolyhedronPolygonsCount();
		virtual ~PolyhedronPolygonsCount();
		virtual float value(vis::Polyhedron* m );
		virtual bool isFullFilled( vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
	protected:
	private:
};

#endif // POLYHEDRONPOLYGONSCOUNT_H
