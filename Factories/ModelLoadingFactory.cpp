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
	for( ModelLoadingStrategy* strategy : modelLoadingStrategies ) {
#ifdef DEBUG_MOD
		std::cout << "Deleting Model Loading Strategy from Factory: ";
		std::cout << strategy->getFileFormats()[0].fileFormatName << std::endl;
#endif
		delete strategy;
	}
}
bool ModelLoadingFactory::addNewModelLoadingStrategy( ModelLoadingStrategy* ml){
	bool added = false;
	for( AcceptedFileFormat& fileFormat : ml->getFileFormats() ){
		this->registerInstance(fileFormat.fileFormatExt,ml);
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
	std::cout << "File: " << filename << ", Extension: " << ext << "\n";
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
	std::cout << "File: " << filename << ", Extension: " << ext << "\n";
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

