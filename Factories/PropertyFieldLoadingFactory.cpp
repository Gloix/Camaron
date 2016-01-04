#include "Factories/PropertyFieldLoadingFactory.h"
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <stdio.h>
#include "Common/Constants.h"
PropertyFieldLoadingFactory* PropertyFieldLoadingFactory::instance = (PropertyFieldLoadingFactory*)0;
PropertyFieldLoadingFactory::PropertyFieldLoadingFactory(): RegistryTemplate<std::string, PropertyFieldLoadingStrategy>()
{
}

PropertyFieldLoadingFactory::~PropertyFieldLoadingFactory()
{
	for( PropertyFieldLoadingStrategy* strategy : propertyFieldLoadingStrategies) {

#ifdef DEBUG_MOD
		std::cout << "Deleting Property Field Loading Strategy from Factory: ";
		std::cout << strategy->getFileFormats()[0].fileFormatName << std::endl;
#endif
		delete strategy;
	}
}
bool PropertyFieldLoadingFactory::addNewPropertyFieldLoadingStrategy( PropertyFieldLoadingStrategy* pfl){
	bool added = false;
	for( AcceptedFileFormat& fileFormat : pfl->getFileFormats()){
		this->registerInstance(fileFormat.fileFormatExt,pfl);
		added = true;
	}
	if(added)
		propertyFieldLoadingStrategies.push_back(pfl);
	else
		delete pfl;
	return added;

}

//void PropertyFieldLoadingFactory::loadPropertyField( std::string filename, Model* model, PropertyFieldDef propertyFieldDef )throw (ModelLoadingException,
//																	ExceptionMessage,
//																	UnknownExtensionException,
//																	std::bad_alloc){
//	if(!FileUtils::fileExists(filename)){
//		throw ExceptionMessage("File not found:<br>"+filename);
//	}
//	std::string ext = FileUtils::getFileNameExtension(filename);

//#ifdef DEBUG_MOD
//	std::cout << "File: " << filename << ", Extension: " << ext << "\n";
//#endif
//	PropertyFieldLoadingStrategy* strategy = this->getRegistryByKeyInstance( ext );
//	if( strategy ) {
//		if( strategy->validate( filename ) )
//			strategy-> load( filename, model, propertyFieldDef);
//	}
//	throw UnknownExtensionException(filename);
//}
PropertyFieldLoadingStrategy* PropertyFieldLoadingFactory::loadPropertyFieldQThread( std::string filename )throw (ModelLoadingException,
																	ExceptionMessage,
																	UnknownExtensionException,
																	std::bad_alloc){
	if(!FileUtils::fileExists(filename)){
		throw ExceptionMessage("File not found:<br>"+filename);
	}
	std::string ext = FileUtils::getFileNameExtension(filename);

#ifdef DEBUG_MOD
	std::cout << "File: " << filename << ", Extension: " << ext << "\n";
#endif
	PropertyFieldLoadingStrategy* strategy = this->getRegistryByKeyInstance( ext );
	if( strategy ) {
		if( strategy->validate( filename ) ){
			return strategy;
		}
	}
	throw UnknownExtensionException(filename);
}
std::vector<PropertyFieldLoadingStrategy*>& PropertyFieldLoadingFactory::getPropertyFieldLoadingStrategies(){
	return propertyFieldLoadingStrategies;
}

PropertyFieldLoadingFactory* PropertyFieldLoadingFactory::getSingletonInstance(){
	if(!PropertyFieldLoadingFactory::instance)
		PropertyFieldLoadingFactory::instance = new PropertyFieldLoadingFactory();
	return PropertyFieldLoadingFactory::instance;
}
void PropertyFieldLoadingFactory::deleteSingletonInstance(){
	if(PropertyFieldLoadingFactory::instance)
		delete PropertyFieldLoadingFactory::instance;
}

