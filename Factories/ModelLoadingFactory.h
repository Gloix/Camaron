#ifndef MODELLOADINGFACTORY_H
#define MODELLOADINGFACTORY_H
#include <vector>
#include "RegistryTemplate.h"
#include "Exceptions/modelloadingexception.h"

#define REGISTER_MODEL_LOADING_STRATEGY(x) \
	int x##_dummy_var  = \
	ModelLoadingFactory::getSingletonInstance()->\
	addNewModelLoadingStrategy(new x())

#define REGISTER_LIGHT_WEIGHT_MODEL_LOADING_STRATEGY(x) \
	int x##_dummy_var  = \
	ModelLoadingFactory::getLightWeightSingletonInstance()->\
	addNewModelLoadingStrategy(new x())
class ModelLoadingStrategy;
class Model;
class ModelLoadingFactory: public RegistryTemplate<std::string,ModelLoadingStrategy> {
	public:
		ModelLoadingFactory();
		virtual ~ModelLoadingFactory();
		static ModelLoadingFactory* getSingletonInstance();
		static ModelLoadingFactory* getLightWeightSingletonInstance();
		static void deleteSingletonInstances();
		Model* loadModel( std::string ) throw (ModelLoadingException,
											   ExceptionMessage,
											   UnknownExtensionException,
											   std::bad_alloc);

		std::vector<ModelLoadingStrategy*>& getModelLoadingStrategies();
		bool addNewModelLoadingStrategy( ModelLoadingStrategy*);
		ModelLoadingStrategy* loadModelQThread( std::string ) throw (ModelLoadingException,
																	ExceptionMessage,
																	UnknownExtensionException,
																	std::bad_alloc);
	protected:
	private:
		static ModelLoadingFactory* instance;
		static ModelLoadingFactory* instanceLW;
		std::vector<ModelLoadingStrategy*> modelLoadingStrategies;
};

#endif // MODELLOADINGFACTORY_H
