#include "crosstimer.h"

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
