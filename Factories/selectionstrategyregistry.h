#ifndef SELECTIONSTRATEGYREGISTRY_H
#define SELECTIONSTRATEGYREGISTRY_H
#include "RegistryTemplate.h"
#include "SelectionStrategies/SelectionStrategy.h"

#define REGISTER_SELECTION_STRATEGY(x) \
	int x##_dummy_var  = \
	SelectionStrategyRegistry::getSingletonInstance()->\
	insertNextSelectionStrategy(new x())
#define REGISTER_SELECTION_STRATEGY_INSTANCE(x) \
	int x##_dummy_var_selection_inst  = \
	SelectionStrategyRegistry::getSingletonInstance()->\
	insertNextSelectionStrategy(x)
class SelectionStrategyRegistry: public RegistryTemplate<int,SelectionStrategy>
{
	public:
		virtual ~SelectionStrategyRegistry();
		int insertNextSelectionStrategy(SelectionStrategy*);
		static SelectionStrategyRegistry* getSingletonInstance();
		static void deleteSingletonInstance();
	private:
		int nextKey;
		SelectionStrategyRegistry();
		static SelectionStrategyRegistry* instance;
};

#endif // SELECTIONSTRATEGYREGISTRY_H
