#ifndef POLYGONAREA_H
#define POLYGONAREA_H
#include "EvaluationStrategies/EvaluationStrategy.h"

class PolygonArea: public EvaluationStrategy
{
	public:
		PolygonArea();
		virtual ~PolygonArea();
		virtual float value(vis::Polygon* m );
		virtual bool isFullFilled( vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
	protected:
	private:
};

#endif // AREA_H
