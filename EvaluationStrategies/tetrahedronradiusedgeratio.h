#ifndef TETRAHEDRONRADIUSEDGERATIO_H
#define TETRAHEDRONRADIUSEDGERATIO_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class TetrahedronRadiusEdgeRatio: public EvaluationStrategy
{
	public:
		TetrahedronRadiusEdgeRatio();
		virtual ~TetrahedronRadiusEdgeRatio();
		virtual float value(vis::Polyhedron* m );
		virtual bool isFullFilled( vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // TETRAHEDRONRADIUSEDGERATIO_H
