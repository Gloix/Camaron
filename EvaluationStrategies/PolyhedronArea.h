#ifndef POLYHEDRONAREA_H
#define POLYHEDRONAREA_H
#include "EvaluationStrategies/EvaluationStrategy.h"
class PolyhedronArea: public EvaluationStrategy
{
	public:
		PolyhedronArea();
		virtual ~PolyhedronArea();
		virtual float value( vis::Polyhedron* m );
		virtual bool isFullFilled( vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
	private:
};

#endif // POLYHEDRONAREA_H
