#ifndef PROPERTYFIELDLOADINGFACTORY_H
#define PROPERTYFIELDLOADINGFACTORY_H
#include <vector>
#include "RegistryTemplate.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/VertexCloud.h"

#define REGISTER_PROPERTY_FIELD_LOADING_STRATEGY(x) \
	int x##_dummy_var  = \
	PropertyFieldLoadingFactory::getSingletonInstance()->\
	addNewPropertyFieldLoadingStrategy(new x())

struct PropertyFieldDef;
class PropertyFieldLoadingStrategy;
class Model;
class PropertyFieldLoadingFactory: public RegistryTemplate<std::string,PropertyFieldLoadingStrategy> {
	public:
		PropertyFieldLoadingFactory();
		virtual ~PropertyFieldLoadingFactory();
		static PropertyFieldLoadingFactory* getSingletonInstance();
		static void deleteSingletonInstance();
//		void loadPropertyField( std::string, Model* model, PropertyFieldDef propertyFieldDef) throw (ModelLoadingException,
//													   ExceptionMessage,
//													   UnknownExtensionException,
//													   std::bad_alloc);

		std::vector<PropertyFieldLoadingStrategy*>& getPropertyFieldLoadingStrategies();
		bool addNewPropertyFieldLoadingStrategy( PropertyFieldLoadingStrategy*);
		PropertyFieldLoadingStrategy* loadPropertyFieldQThread( std::string )
				throw (ModelLoadingException,
					   ExceptionMessage,
					   UnknownExtensionException,
					   std::bad_alloc);
	protected:
	private:
		static PropertyFieldLoadingFactory* instance;
		std::vector<PropertyFieldLoadingStrategy*> propertyFieldLoadingStrategies;
};

#endif // PROPERTYFIELDLOADINGFACTORY_H
