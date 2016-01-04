#include "RendererRegistry.h"
#include "Common/Constants.h"
#include <vector>
#include <iostream>
RendererRegistry* RendererRegistry::instance = (RendererRegistry*)0;

RendererRegistry::RendererRegistry()
{
	nextKey = 0;
}
RendererRegistry::~RendererRegistry(){
	typedef std::map<int, Renderer*>::iterator it_type;
	for(it_type iterator = this->registryMap->begin(); iterator != this->registryMap->end(); iterator++) {
		// iterator->first = key
		Renderer* p = iterator->second;
		if(std::string("Geometry Intersection").compare(p->getRendererName())){
#ifdef DEBUG_MOD
		std::cout << "Deleting Renderer from RendererRegistry: " << p->getRendererName() << std::endl;
#endif
		delete p;
		}
	}
}
bool RendererRegistry::insertRenderer(Renderer* r){
	toInsert.push_back(r);
	return true;
}
void RendererRegistry::glewIsReady(){
	for( Renderer* renderer : toInsert ){
		renderer->glewIsReadyRenderer();
		//if(r->isOk())
			insertNextRenderer(renderer);
		//else
		//	delete r;
	}

}

bool RendererRegistry::insertNextRenderer(Renderer* renderer){
/*	if(!renderer->isOk()){
#ifdef DEBUG_MOD
		std::cout << "Deleting Renderer from RendererRegistry (NOT OK): " << renderer->getRendererName() << std::endl;
#endif
		delete renderer;
		return false;
	}else{*/
		this->registerInstance(nextKey++,renderer,renderer->getRendererWeight());
		return true;
	//}
}
Renderer* RendererRegistry::getByName(std::string search){
	typedef std::map<int, Renderer*>::iterator it_type;
	for(it_type iterator = this->registryMap->begin(); iterator != this->registryMap->end(); iterator++) {
		Renderer* p = iterator->second;
		if(!search.compare(p->getRendererName()))
			return p;
	}
	return (Renderer*)0;
}
RendererRegistry* RendererRegistry::getSingletonInstance(){
	if(!RendererRegistry::instance)
		RendererRegistry::instance = new RendererRegistry();
	return RendererRegistry::instance;
}

void RendererRegistry::deleteSingletonInstance(){
	if(RendererRegistry::instance)
		delete RendererRegistry::instance;
}
