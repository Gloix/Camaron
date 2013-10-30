#ifndef REGISTRYTEMPLATE_H
#define REGISTRYTEMPLATE_H

#include <string>
#include <map>

#define REGISTER_DEC_TYPE(NAME) \
	static RegistryTemplate<NAME> reg

#define REGISTER_DEF_TYPE(NAME) \
	RegistryTemplate<NAME> NAME::reg(#NAME)


template<class KEY,class T>
class RegistryTemplate
{
	public:
		RegistryTemplate();
		virtual ~RegistryTemplate();
		void registerInstance( KEY, T* );
		void registerInstance( KEY, T*, float weight );
		T* getRegistryByKeyInstance( KEY );
		std::map<KEY, T*> * getRegistryMap( );
		std::map<float, KEY> * getWeightMap( );
	protected:
		std::map<KEY, T*> * registryMap;
		std::map<float, KEY> * weightMap;//to order by weight
		float getNextUnusedWeight();
	private:
		static float incrementFloatWeight(float f);

};

template <class KEY,class T>
std::map<KEY, T*> * RegistryTemplate<KEY,T>::getRegistryMap( ){
	return this->registryMap;
}

template <class KEY,class T>
std::map<float, KEY> * RegistryTemplate<KEY,T>::getWeightMap( ){
	return this->weightMap;
}

template <class KEY,class T>
RegistryTemplate<KEY,T>::RegistryTemplate()
{
	this->registryMap = new std::map<KEY, T*>();
	weightMap = new std::map<float,KEY>();
}

template <class KEY,class T>
RegistryTemplate<KEY,T>::~RegistryTemplate()
{
	delete this->registryMap;
	delete weightMap;
}

template <class KEY,class T>
float RegistryTemplate<KEY,T>::incrementFloatWeight(float f){
	float lastVal = f;
	float newVal = f*1.1f;
	if(lastVal==newVal)
		newVal = f+1.0f;
	return newVal;
}

template <class KEY,class T>
void RegistryTemplate<KEY,T>::registerInstance( KEY s, T* t, float weight  ) {
	this->registryMap->insert( std::make_pair( s, t ) );
	while(weightMap->count(weight)>0)//weight taken
		weight = incrementFloatWeight(weight);//increment
	weightMap->insert( std::make_pair( weight, s ) );
}

template <class KEY,class T>
void RegistryTemplate<KEY,T>::registerInstance( KEY s, T* t ) {
	this->registryMap->insert( std::make_pair( s, t ) );
	int w = getNextUnusedWeight();
	weightMap->insert( std::make_pair( w, s ) );
}
template <class KEY,class T>
T* RegistryTemplate<KEY,T>::getRegistryByKeyInstance( KEY s ) {
	typename std::map<KEY, T*>::iterator it = this->registryMap->find( s );
	if( it == this->registryMap->end() )
		return ( T* )0;
	return it->second;
}
template <class KEY,class T>
float RegistryTemplate<KEY,T>::getNextUnusedWeight(){
	if(weightMap->size()==0)
		return 1.0f;//min default weight
	typename std::map<float,KEY>::iterator it = this->weightMap->end();
	it--;
	return incrementFloatWeight(it->first);
}

#endif // REGISTRYTEMPLATE_H
