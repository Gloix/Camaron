#include "modelexportstrategyregistry.h"
#include "ModelExport/modelexportstrategy.h"
#include "Utils/fileutils.h"
#include "Common/Constants.h"
#include "SelectionStrategies/Selection.h"
#include <iostream>
ModelExportStrategyRegistry* ModelExportStrategyRegistry::instance = (ModelExportStrategyRegistry*)0;

ModelExportStrategyRegistry::ModelExportStrategyRegistry()
{
}

ModelExportStrategyRegistry::~ModelExportStrategyRegistry(){
	typedef std::map<std::string, ModelExportStrategy*>::iterator it_type;
	for(it_type iterator = this->registryMap->begin(); iterator != this->registryMap->end(); iterator++) {
		// iterator->first = key
		ModelExportStrategy* p = iterator->second;

#ifdef DEBUG_MOD
		std::cout << "Deleting Model Export Strategy from Factory: " << p->getName() << std::endl;
#endif
		delete p;
	}
}

ModelExportStrategyRegistry* ModelExportStrategyRegistry::getSingletonInstance(){
	if(!ModelExportStrategyRegistry::instance)
		ModelExportStrategyRegistry::instance = new ModelExportStrategyRegistry();
	return ModelExportStrategyRegistry::instance;
}
int ModelExportStrategyRegistry::insertModelExportStrategy(ModelExportStrategy* strategy){
	registerInstance(strategy->getExtension(),strategy);
	return 0;
}

bool ModelExportStrategyRegistry::exportModel(Model* m, std::string filename)  throw (ExceptionMessage,UnknownExtensionException){
	//validation
	std::string extension = FileUtils::getFileNameExtension(filename);
	ModelExportStrategy* exportStrategy = getRegistryByKeyInstance(extension);
	if(exportStrategy)
		return exportStrategy->exportModel(m,filename);
	throw UnknownExtensionException(filename);
}
bool ModelExportStrategyRegistry::exportSelection(Selection* m, std::string filename){
	//validation
	if(m->getSelectedElements().size()==0)
		throw ExceptionMessage("Can't export selection with no selected elements.");
	std::string extension = FileUtils::getFileNameExtension(filename);
	ModelExportStrategy* exportStrategy = getRegistryByKeyInstance(extension);
	if(exportStrategy)
		return exportStrategy->exportSelection(m,filename);
	throw UnknownExtensionException(filename);
}
void ModelExportStrategyRegistry::deleteSingletonInstance(){
	if(ModelExportStrategyRegistry::instance)
		delete ModelExportStrategyRegistry::instance;
}

