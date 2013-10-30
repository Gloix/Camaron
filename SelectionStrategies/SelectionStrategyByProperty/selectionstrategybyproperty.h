#ifndef SELECTIONSTRATEGYBYPROPERTY_H
#define SELECTIONSTRATEGYBYPROPERTY_H
#include "SelectionStrategies/SelectionStrategy.h"
class EvaluationStrategy;
class SelectionStrategyByPropertyConfig;
class SelectionStrategyByProperty: public SelectionStrategy
{
	public:
		SelectionStrategyByProperty();
		virtual ~SelectionStrategyByProperty();
		virtual bool selectElement( vis::Polyhedron *, Selection* );
		virtual bool selectElement( vis::Polygon *, Selection* );
		virtual bool selectElement( vis::Vertex *, Selection* );
		//depends on the evaluation strategy
		virtual bool isFullFilled( vis::Vertex * v );
		virtual bool isFullFilled( vis::Polygon * m );
		virtual bool isFullFilled( vis::Polyhedron * m );
		virtual bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
		virtual bool hasConfigWidget();
		virtual void QApplicationInitiated();
		virtual QWidget* getConfigWidget();
	protected:
	private:
		EvaluationStrategy* evaluationStrategy;
		float minVal, maxVal;
		SelectionStrategyByPropertyConfig* config;
};

#endif // SELECTIONSTRATEGYBYPROPERTY_H
