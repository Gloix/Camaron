#include "selectionstrategyregistry.h"
#include "SelectionStrategies/SelectionStrategy.h"

SelectionStrategyRegistry* SelectionStrategyRegistry::instance = (SelectionStrategyRegistry*)0;

SelectionStrategyRegistry::SelectionStrategyRegistry(): RegistryTemplate<int,SelectionStrategy>()
{
	nextKey = 0;
}
SelectionStrategyRegistry::~SelectionStrategyRegistry(){
	typedef std::map<int, SelectionStrategy*>::iterator it_type;
	for(it_type iterator = this->registryMap->begin(); iterator != this->registryMap->end(); iterator++) {
		// iterator->first = key
		SelectionStrategy* p = iterator->second;

#ifdef DEBUG_MOD
		std::cout << "Deleting Selection Strategy from Factory: " << p->getName() << std::endl;
#endif
		delete p;
	}
}
int SelectionStrategyRegistry::insertNextSelectionStrategy(SelectionStrategy* sel){
	this->registerInstance(nextKey++,sel,sel->getWeight());
	return nextKey-1;
}

SelectionStrategyRegistry* SelectionStrategyRegistry::getSingletonInstance(){
	if(!SelectionStrategyRegistry::instance)
		SelectionStrategyRegistry::instance = new SelectionStrategyRegistry();
	return SelectionStrategyRegistry::instance;
}

void SelectionStrategyRegistry::deleteSingletonInstance(){
	if(SelectionStrategyRegistry::instance)
		delete SelectionStrategyRegistry::instance;
}
