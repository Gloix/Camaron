#include "crosstimer.h"
#include <iostream>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>

CrossTimer::CrossTimer(){
	if(!QueryPerformanceFrequency(&pcFreq))
		std::cout << "QueryPerformanceFrequency failed!\n";

	QueryPerformanceCounter(&startTime);
}
long CrossTimer::getTranscurredMilliseconds(){
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	result.QuadPart-=startTime.QuadPart;
	result.QuadPart *= 1000;
	return long(result.QuadPart/pcFreq.QuadPart);
}

float CrossTimer::getTranscurredSeconds(){
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	result.QuadPart-=startTime.QuadPart;
	return float(result.QuadPart)/pcFreq.QuadPart;
}
long CrossTimer::getTranscurredNanoseconds(){
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	result.QuadPart-=startTime.QuadPart;
	result.QuadPart *= 1000000000;
	return long(result.QuadPart/pcFreq.QuadPart);
}
#else


CrossTimer::CrossTimer():createdAt(std::chrono::high_resolution_clock::now()){

	//t = clock();
}
long CrossTimer::getTranscurredMilliseconds(){
	auto t2 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(t2-createdAt).count();
}

float CrossTimer::getTranscurredSeconds(){
	return this->getTranscurredMilliseconds()/1000.0f;
	//return ((float)(clock()-t))/CLOCKS_PER_SEC;
}
long CrossTimer::getTranscurredNanoseconds(){
	auto t2 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(t2-createdAt).count();
}
#endif
