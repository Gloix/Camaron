#ifndef CHANGESELECTIONTYPE_H
#define CHANGESELECTIONTYPE_H
#include "SelectionStrategies/SelectionStrategy.h"
class ChangeSelectionTypeConfig;
class ChangeSelectionType: public SelectionStrategy
{
	public:
		ChangeSelectionType();
		virtual ~ChangeSelectionType();
		virtual void selectElement( Selection* );
		//depends on the evaluation strategy
		virtual bool isFullFilled( Selection * );
		virtual bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
		virtual bool hasConfigWidget();
		virtual void QApplicationInitiated();
		virtual QWidget* getConfigWidget();
	private:
		ChangeSelectionTypeConfig* config;
};

#endif // CHANGESELECTIONTYPE_H
