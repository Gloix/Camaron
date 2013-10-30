#include "Factories/ModelLoadingFactory.h"
#include "ModelLoading/ModelLoadingOff.h"
#include "ModelLoading/modelloadingelenode.h"
#include "ModelLoading/modelloadingts.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <stdio.h>
#include "Common/Constants.h"
ModelLoadingFactory* ModelLoadingFactory::instance = (ModelLoadingFactory*)0;
ModelLoadingFactory* ModelLoadingFactory::instanceLW = (ModelLoadingFactory*)0;
ModelLoadingFactory::ModelLoadingFactory(): RegistryTemplate<std::string, ModelLoadingStrategy>()
{
}

ModelLoadingFactory::~ModelLoadingFactory()
{
	typedef std::vector<ModelLoadingStrategy*>::size_type it_type;
	for(it_type iterator = 0; iterator < modelLoadingStrategies.size();iterator++) {
		// iterator->first = key
		ModelLoadingStrategy* p = modelLoadingStrategies[iterator];

#ifdef DEBUG_MOD
		std::cout << "Deleting Model Loading Strategy from Factory: ";
		std::cout << p->getFileFormats()[0].fileFormatName << std::endl;
#endif
		delete p;
	}
}
bool ModelLoadingFactory::addNewModelLoadingStrategy( ModelLoadingStrategy* ml){
	bool added = false;
	for(std::vector<AcceptedFileFormat>::size_type i = 0;i < ml->getFileFormats().size();i++){
		AcceptedFileFormat current = ml->getFileFormats()[i];
		this->registerInstance(current.fileFormatExt,ml);
		added = true;
	}
	if(added)
		modelLoadingStrategies.push_back(ml);
	else
		delete ml;
	return added;

}

Model* ModelLoadingFactory::loadModel( std::string filename )throw (ModelLoadingException,
																	ExceptionMessage,
																	UnknownExtensionException,
																	std::bad_alloc){
	if(!FileUtils::fileExists(filename)){
		throw ExceptionMessage("File not found:<br>"+filename);
	}
	std::string ext = FileUtils::getFileNameExtension(filename);

#ifdef DEBUG_MOD
	std::cout << "Archivo: " << filename << ", Extension: " << ext << "\n";
#endif
	ModelLoadingStrategy* strategy = this->getRegistryByKeyInstance( ext );
	if( strategy ) {
		Model* loadedModel = ( Model* )0;

		if( strategy->validate( filename ) )
			loadedModel = strategy->load( filename );
		return loadedModel;
	}
	throw UnknownExtensionException(filename);
}
ModelLoadingStrategy* ModelLoadingFactory::loadModelQThread( std::string filename )throw (ModelLoadingException,
																	ExceptionMessage,
																	UnknownExtensionException,
																	std::bad_alloc){
	if(!FileUtils::fileExists(filename)){
		throw ExceptionMessage("File not found:<br>"+filename);
	}
	std::string ext = FileUtils::getFileNameExtension(filename);

#ifdef DEBUG_MOD
	std::cout << "Archivo: " << filename << ", Extension: " << ext << "\n";
#endif
	ModelLoadingStrategy* strategy = this->getRegistryByKeyInstance( ext );
	if( strategy ) {
		if( strategy->validate( filename ) ){
			return strategy;
		}
	}
	throw UnknownExtensionException(filename);
}
std::vector<ModelLoadingStrategy*>& ModelLoadingFactory::getModelLoadingStrategies(){
	return modelLoadingStrategies;
}

ModelLoadingFactory* ModelLoadingFactory::getSingletonInstance(){
	if(!ModelLoadingFactory::instance)
		ModelLoadingFactory::instance = new ModelLoadingFactory();
	return ModelLoadingFactory::instance;
}
ModelLoadingFactory* ModelLoadingFactory::getLightWeightSingletonInstance(){
	if(!ModelLoadingFactory::instanceLW)
		ModelLoadingFactory::instanceLW = new ModelLoadingFactory();
	return ModelLoadingFactory::instanceLW;
}
void ModelLoadingFactory::deleteSingletonInstances(){
	if(ModelLoadingFactory::instance)
		delete ModelLoadingFactory::instance;
	if(ModelLoadingFactory::instanceLW)
		delete ModelLoadingFactory::instanceLW;
}

