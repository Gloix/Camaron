#include "renderinglock.h"

RenderingLock::RenderingLock():
	qsemaphore(1)
{

#ifdef WORKING_IN_WINDOWS
	InitializeCriticalSection(&_mutex);
#else
	pthread_mutex_init(&_mutex, NULL);
#endif
	needRendering = true;

}

RenderingLock::~RenderingLock()
{
#ifdef WORKING_IN_WINDOWS
	DeleteCriticalSection(&_mutex);
#else
	pthread_mutex_unlock(&_mutex);
	pthread_mutex_destroy(&_mutex);
#endif
	needRendering = false;
	qsemaphore.release(10);
}

bool RenderingLock::needsRendering(){
	//qsemaphore.acquire((qsemaphore.available()>0)?qsemaphore.available():1);
	bool val = needRendering;
	needRendering = false;
	return val;
	/*
#ifdef WORKING_IN_WINDOWS
	EnterCriticalSection(&_mutex);
	val = needRendering;
	needRendering = false;
	LeaveCriticalSection(&_mutex);
	return val;
#else
	if(pthread_mutex_lock(&_mutex) == 0){

		val = needRendering;
		pthread_mutex_unlock(&_mutex);
		return val;
	}
	return false;
#endif*/
}

void RenderingLock::setRenderingNeed(bool b){
	needRendering = b;
	//qsemaphore.release(1);
/*
#ifdef WORKING_IN_WINDOWS
	EnterCriticalSection(&_mutex);
	needRendering = b;
	LeaveCriticalSection(&_mutex);
#else
	if(pthread_mutex_lock(&_mutex) == 0){

		needRendering = b;
		pthread_mutex_unlock(&_mutex);
	}
#endif*/
}
