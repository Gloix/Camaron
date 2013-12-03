#ifndef POLYGONVERTICESCOUNT_H
#define POLYGONVERTICESCOUNT_H

#include "EvaluationStrategies/EvaluationStrategy.h"
class PolygonVerticesCount: public EvaluationStrategy
{
	public:
		PolygonVerticesCount();
		virtual ~PolygonVerticesCount();
		virtual float value(vis::Polygon* m );
		virtual bool isFullFilled( vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
	protected:
	private:
};

#endif // POLYGONVERTICESCOUNT_H
