#include "hashingtree.h"
#include <algorithm>

/*
template <class KEY,class T>
vis::ut::HashingTree<KEY,T>::HashingTree()
{
//	children = new std::unordered_map<KEY,HashingTree<KEY,T> >();
//	values = new std::unordered_map<KEY,T* >();
//	ordered = true;
}

template <class KEY,class T>
vis::ut::HashingTree<KEY,T>::~HashingTree(){
//	delete children;
//	delete values;
}


template <class KEY,class T>
T* vis::ut::HashingTree<KEY,T>::getRegistry(std::vector<KEY> keys){
//	if(ordered)
//		std::sort(keys.begin(),keys.end());
	return getRegistryIn(keys,0u);
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::registerValue(std::vector<KEY> keys, T* value){
//	if(ordered)
//		std::sort(keys.begin(),keys.end());
	getRegistryIn(keys,0u);
}*/
/*
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::hasChild(KEY key){
	return children.count(key)>0;
}
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::hasValue(KEY key){
	return values.count(key)>0;
}
template <class KEY,class T>
T* vis::ut::HashingTree<KEY,T>::getRegistryIn(std::vector<KEY>& keys,unsigned int current){
	if(keys.size()-1==current){
		typename std::unordered_map<KEY, T*>::iterator it = this->values->find( keys[current] );
		if( it == this->values->end() )
			return ( T* )0;
		return it->second;
	}
	typename std::unordered_map<KEY, HashingTree<KEY,T> >::iterator it =
			this->children->find( keys[current] );
	if( it == this->values->end() )
		return ( T* )0;
	return it->second.getRegistryIn(keys,++current);
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::registerValueIn(std::vector<KEY>& keys, T* value, unsigned int current){
	if(keys.size()-1==current){
		values[keys[current]] = value;
		return;
	}
	children[keys[current]].registerValueIn(keys,value,++current);
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::setOrdered(bool b){
	ordered = b;
}
*/
