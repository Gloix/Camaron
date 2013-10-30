#include "Factories/EvaluationStrategyRegistry.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Common/Constants.h"
#include <iostream>

EvaluationStrategyRegistry* EvaluationStrategyRegistry::instance = (EvaluationStrategyRegistry*)0;

EvaluationStrategyRegistry::EvaluationStrategyRegistry(): RegistryTemplate<unsigned char,EvaluationStrategy>()
{
	nextKey = 0;
}

EvaluationStrategyRegistry::~EvaluationStrategyRegistry()
{
	typedef std::map<unsigned char, EvaluationStrategy*>::iterator it_type;
	for(it_type iterator = this->registryMap->begin(); iterator != this->registryMap->end(); iterator++) {
		// iterator->first = key
		EvaluationStrategy* p = iterator->second;

#ifdef DEBUG_MOD
		std::cout << "Deleting Evaluation Strategy from Factory: " << p->getName() << std::endl;
#endif
		delete p;
	}
}
unsigned char EvaluationStrategyRegistry::insertNextEvaluationStrategy(EvaluationStrategy* eval){
	eval->setId(nextKey);
	this->registerInstance(nextKey++,eval,eval->getWeight());
	return nextKey-1;

}
EvaluationStrategyRegistry* EvaluationStrategyRegistry::getSingletonInstance(){
	if(!EvaluationStrategyRegistry::instance)
		EvaluationStrategyRegistry::instance = new EvaluationStrategyRegistry();
	return EvaluationStrategyRegistry::instance;
}

void EvaluationStrategyRegistry::deleteSingletonInstance(){
	if(EvaluationStrategyRegistry::instance)
		delete EvaluationStrategyRegistry::instance;
}
