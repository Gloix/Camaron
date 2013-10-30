#ifndef HASHINGTREE_H
#define HASHINGTREE_H

#include <vector>
#include <algorithm>
#include <unordered_map>
namespace vis{
namespace ut{
template<class KEY,class T>
class HashingTree
{
	public:
		HashingTree();
		virtual ~HashingTree();
		T* getRegistry(std::vector<KEY>);
		void setOrdered(bool);
		void registerValue(std::vector<KEY> keys, T* value);

	private:
		bool hasChild(KEY);
		bool hasValue(KEY);
		bool isEmpty();
		T* getRegistryIn(std::vector<KEY>&,unsigned int);
		void registerValueIn(std::vector<KEY>& keys, T* value,unsigned int);
		std::unordered_map<KEY,HashingTree<KEY,T>* > children;
		std::unordered_map<KEY,T*> values;
		bool ordered;
};
}
}


template <class KEY,class T>
vis::ut::HashingTree<KEY,T>::HashingTree()
{
	ordered = true;
}

template <class KEY,class T>
vis::ut::HashingTree<KEY,T>::~HashingTree(){
	typename std::unordered_map<KEY, vis::ut::HashingTree<KEY,T>* >::iterator it =
			children.begin();
	while(it!=children.end()){
		vis::ut::HashingTree<KEY,T>* p = it->second;
		delete p;
		it++;
	}
}


template <class KEY,class T>
T* vis::ut::HashingTree<KEY,T>::getRegistry(std::vector<KEY> keys){
	if(ordered)
		std::sort(keys.begin(),keys.end());
	return getRegistryIn(keys,0u);
}
template <class KEY,class T>
T* vis::ut::HashingTree<KEY,T>::getRegistryIn(std::vector<KEY>& keys,unsigned int current){
	if(keys.size()-1==current){
		typename std::unordered_map<KEY, T*>::iterator it = this->values.find( keys[current] );
		if( it == this->values.end() )
			return ( T* )0;
		T* found = it->second;
		values.erase(keys[current]);
		return found;
	}
	typename std::unordered_map<KEY, vis::ut::HashingTree<KEY,T>* >::iterator it =
			this->children.find( keys[current] );
	if( it == this->children.end() )
		return ( T* )0;
	T* found = it->second->getRegistryIn(keys,++current);
	vis::ut::HashingTree<KEY,T>* registryToTest = it->second;
	if(registryToTest->isEmpty()){
		children.erase(it->first);
		delete registryToTest;
	}
	return found;
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::registerValue(std::vector<KEY> keys, T* value){
	if(ordered)
		std::sort(keys.begin(),keys.end());
	registerValueIn(keys,value,0u);
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::registerValueIn(std::vector<KEY>& keys, T* value, unsigned int current){
	if(keys.size()-1==current){
		values[keys[current]] = value;
		return;
	}

	vis::ut::HashingTree<KEY,T>* hash;
	if(hasChild(keys[current])){
		hash = children[keys[current]];
	}
	else{
		hash = new vis::ut::HashingTree<KEY,T>();
		children[keys[current]] = hash;
	}
	hash->registerValueIn(keys,value,++current);
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::setOrdered(bool b){
	ordered = b;
}
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::hasChild(KEY key){
	return children.count(key)>0;
}
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::hasValue(KEY key){
	return values.count(key)>0;
}
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::isEmpty(){
	return values.empty()&&children.empty();
}

#endif // HASHINGTREE_H
