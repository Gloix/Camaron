#include "dynamicorderedcontainerwithmaxsize.h"

DynamicOrderedContainerWithMaxSize::DynamicOrderedContainerWithMaxSize()
{
}
void DynamicOrderedContainerWithMaxSize::setCapacity(unsigned int max){
	clear();
	maxCapacity = max;
	orderedElements.reserve(max);
}
unsigned int DynamicOrderedContainerWithMaxSize::getCapacity(){
	return maxCapacity;
}

void DynamicOrderedContainerWithMaxSize::addElement(glm::vec3 vec, int id){
	if(nItems==maxCapacity && vec.z >= maxValue)
		return;//dont add, it will be removed
	if(storage.count(vec.z)>0){
		std::vector<std::pair<glm::vec2,int> >& ss = storage[vec.z];
		ss.push_back(std::make_pair(glm::vec2(vec),id));
	}
	else{
		std::vector<std::pair<glm::vec2,int> > n;
		n.push_back(std::make_pair(glm::vec2(vec),id));
		storage.insert(std::make_pair(vec.z,n));
	}
	maxValue = std::max(vec.z,maxValue);
	if(nItems != maxCapacity){
		nItems++;
		return;
	}
	deleteSmallestElement();
}
void DynamicOrderedContainerWithMaxSize::deleteSmallestElement(){
	std::map<float,std::vector<std::pair<glm::vec2,int> > >::reverse_iterator revIt = storage.rbegin();
	std::vector<std::pair<glm::vec2,int> >& ss = revIt->second;
	if(ss.size()>1){
		ss.pop_back();
	}
	else{
		storage.erase(revIt->first);
		revIt = storage.rbegin();
		maxValue = revIt->first;
	}

}

unsigned int DynamicOrderedContainerWithMaxSize::getSize(){
	return nItems;
}

std::vector<std::pair<glm::vec2,int> >& DynamicOrderedContainerWithMaxSize::getOrderedElements(){
	if(orderedElements.size()!=nItems){
		orderedElements.clear();
		for(std::map<float,std::vector<std::pair<glm::vec2,int> > >::iterator it = storage.begin();
			it!=storage.end();it++){
			std::vector<std::pair<glm::vec2,int> >& current = it->second;
			for(std::vector<std::pair<glm::vec2,int> >::size_type j = 0;j<current.size();j++)
				orderedElements.push_back(current[j]);
		}
	}
	return orderedElements;
}
void DynamicOrderedContainerWithMaxSize::clear(){
	nItems = 0;
	orderedElements.clear();
	storage.clear();
	maxValue = -std::numeric_limits<float>::max();
}
