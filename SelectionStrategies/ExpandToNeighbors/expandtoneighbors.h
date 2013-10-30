#ifndef EXPANDTONEIGHBORS_H
#define EXPANDTONEIGHBORS_H
#include "SelectionStrategies/SelectionStrategy.h"
class ExpandToNeighborsConfig;
class ExpandToNeighbors: public SelectionStrategy
{
	public:
		ExpandToNeighbors();
		virtual ~ExpandToNeighbors	();
		virtual void selectElement( Selection* );
		//depends on the evaluation strategy
		virtual bool isFullFilled( Selection * );
		virtual bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
		virtual bool hasConfigWidget();
		virtual void QApplicationInitiated();
		virtual QWidget* getConfigWidget();
	protected:
	private:
		void expandSelectionSurface(Selection*);
		void expandSelectionSurfaceWithAngle(Selection*, float angle);
		void expandSelectionAll(Selection*);
		ExpandToNeighborsConfig* config;
};
#endif
