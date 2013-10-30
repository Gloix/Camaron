#include "elementutils.h"
#include "Model/Element/Element.h"
#include <iomanip>
#include <limits>
#include <sstream>
#include "Factories/EvaluationStrategyRegistry.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
EvaluationStrategyRegistry* ElementUtils::instance =
		EvaluationStrategyRegistry::getSingletonInstance();

std::string ElementUtils::getPropertiesDisplay(vis::Element* elem){
	LowMemoryHash<unsigned char, float>& properties = elem->getProperties();
	std::stringstream ss;
	ss << std::setprecision(std::numeric_limits<float>::digits10+1);
	bool first = true;
	int i = 0;
	for(LowMemoryHashItem<unsigned char, float>* item = properties.getItemAt(0);
		item&&item->isValid();item=properties.getItemAt(++i)){
		if(!first)
			ss << "\n";
		else
			first = false;
		ss << instance->getRegistryByKeyInstance(item->key)->getPropertyName();
		ss << ": " << item->value;
	}
	return ss.str();
}

ElementUtils::ElementUtils()
{
}
