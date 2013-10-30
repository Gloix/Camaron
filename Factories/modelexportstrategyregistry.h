#ifndef MODELEXPORTFACTORY_H
#define MODELEXPORTFACTORY_H

#include "RegistryTemplate.h"
#include "Exceptions/exceptionmessage.h"
#include "Exceptions/unknownextensionexception.h"


#define REGISTER_MODEL_EXPORT_STRATEGY(x) \
	int x##_dummy_var  = \
	ModelExportStrategyRegistry::getSingletonInstance()->\
	insertModelExportStrategy(new x())

class ModelExportStrategy;
class Model;
class Selection;
class ModelExportStrategyRegistry:public RegistryTemplate<std::string,ModelExportStrategy>
{
	public:
		virtual ~ModelExportStrategyRegistry();
		static ModelExportStrategyRegistry* getSingletonInstance();
		static void deleteSingletonInstance();
		int insertModelExportStrategy(ModelExportStrategy*);
		bool exportModel(Model*, std::string filename) throw (ExceptionMessage,UnknownExtensionException);
		bool exportSelection(Selection*, std::string filename);
	private:
		ModelExportStrategyRegistry();
		static ModelExportStrategyRegistry* instance;
};

#endif // MODELEXPORTFACTORY_H
