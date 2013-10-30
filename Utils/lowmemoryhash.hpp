#ifndef LOWMEMORYHASH_H
#define LOWMEMORYHASH_H
#include <limits>
template <class KEY, class VALUE_TYPE>
struct LowMemoryHashItem{
	public:
		LowMemoryHashItem(){key = std::numeric_limits<KEY>::max();}
		KEY key;
		VALUE_TYPE value;
		bool isValid(){return key != std::numeric_limits<KEY>::max();}

};

template <class KEY, class VALUE_TYPE>
class LowMemoryHash
{
	public:
		LowMemoryHash();
		virtual ~LowMemoryHash();
		int getItemsCount();//O(n)
		int getCapacity();
		bool insert(KEY, VALUE_TYPE);
		bool hasItem(KEY);
		LowMemoryHashItem<KEY,VALUE_TYPE>* getItem(KEY);
		LowMemoryHashItem<KEY,VALUE_TYPE>* operator[] (KEY);
		LowMemoryHashItem<KEY,VALUE_TYPE>* getItemAt(int);

	private:
		int nItems;
		LowMemoryHashItem<KEY,VALUE_TYPE>* items;
		void expand();
		int nextPos();

};
template <class KEY, class VALUE_TYPE>
LowMemoryHash<KEY,VALUE_TYPE>::LowMemoryHash()
{
	nItems = 3;
	items = new LowMemoryHashItem<KEY,VALUE_TYPE>[nItems];
}


template <class KEY, class VALUE_TYPE>
bool LowMemoryHash<KEY,VALUE_TYPE>::insert(KEY key, VALUE_TYPE value){
	for(int i = 0;i<nItems;i++){
		if(!items[i].isValid()){
			items[i].key = key;
			items[i].value = value;
			return true;
		}
	}
	//there was no room
	this->expand();
	return insert(key,value);
}
template <class KEY, class VALUE_TYPE>
void LowMemoryHash<KEY,VALUE_TYPE>::expand(){
	int newItemSize = nItems+3;
	LowMemoryHashItem<KEY,VALUE_TYPE>* newItems = new LowMemoryHashItem<KEY,VALUE_TYPE>[newItemSize];
	for(int i = 0;i<nItems;i++)
		newItems[i] = items[i];
	delete[] items;
	items = newItems;
	nItems = newItemSize;
}
template <class KEY, class VALUE_TYPE>
LowMemoryHash<KEY,VALUE_TYPE>::~LowMemoryHash()
{
	delete[] items;
}

template <class KEY, class VALUE_TYPE>
int LowMemoryHash<KEY,VALUE_TYPE>::getCapacity(){
	return nItems;
}

template <class KEY, class VALUE_TYPE>
int LowMemoryHash<KEY,VALUE_TYPE>::getItemsCount(){
	return nItems;
}

template <class KEY, class VALUE_TYPE>
bool LowMemoryHash<KEY,VALUE_TYPE>::hasItem(KEY k){
	for(int i = 0;i<nItems;i++){
		if(items[i].isValid()){
			if(items[i].key == k)
				return true;
		}else
			return false;
	}
	return false;
}

template <class KEY, class VALUE_TYPE>
LowMemoryHashItem<KEY,VALUE_TYPE>* LowMemoryHash<KEY,VALUE_TYPE>::getItem(KEY k){
	for(int i = 0;i<nItems;i++){
		if(items[i].isValid()){
			if(items[i].key == k)
				return &items[i];
		}else
			return (LowMemoryHashItem<KEY,VALUE_TYPE>*)0;
	}
	return (LowMemoryHashItem<KEY,VALUE_TYPE>*)0;
}
template <class KEY, class VALUE_TYPE>
int LowMemoryHash<KEY,VALUE_TYPE>::nextPos(){
	for(int i = 0;i<nItems;i++){
		if(!items[i].isValid())
			return i;
	}
	return -1;//no room
}

template <class KEY, class VALUE_TYPE>
LowMemoryHashItem<KEY,VALUE_TYPE>* LowMemoryHash<KEY,VALUE_TYPE>::operator[] (KEY k){
	return getItem(k);
}
template <class KEY, class VALUE_TYPE>
LowMemoryHashItem<KEY,VALUE_TYPE>* LowMemoryHash<KEY,VALUE_TYPE>::getItemAt(int k){
	if(k<nItems)
		return &items[k];
	return (LowMemoryHashItem<KEY,VALUE_TYPE>*)0;
}

#endif // LOWMEMORYHASH_H
