#ifndef EVALUATIONSTRATEGYREGISTRY_H
#define EVALUATIONSTRATEGYREGISTRY_H
#include "RegistryTemplate.h"


#define REGISTER_EVALUATION_STRATEGY(x) \
	int x##_dummy_var  = \
	EvaluationStrategyRegistry::getSingletonInstance()->\
	insertNextEvaluationStrategy(new x())

class EvaluationStrategy;
class EvaluationStrategyRegistry: public RegistryTemplate<unsigned char,EvaluationStrategy>
{
	public:
		virtual ~EvaluationStrategyRegistry();
		static EvaluationStrategyRegistry* getSingletonInstance();
		static void deleteSingletonInstance();
		unsigned char insertNextEvaluationStrategy(EvaluationStrategy*);
	protected:
	private:
		EvaluationStrategyRegistry();
		unsigned char nextKey;
		static EvaluationStrategyRegistry* instance;
};

#endif // EVALUATIONSTRATEGYREGISTRY_H
