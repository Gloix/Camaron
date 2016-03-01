#ifndef CROSSTIMER_H
#define CROSSTIMER_H
#include <chrono>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif
//#include <ctime>
class CrossTimer
{
	public:
		CrossTimer();
		long getTranscurredMilliseconds();
		float getTranscurredSeconds();
		long getTranscurredNanoseconds();
	private:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		LARGE_INTEGER pcFreq;
		LARGE_INTEGER startTime;
#else
		std::chrono::time_point<std::chrono::system_clock> createdAt;
		//int t;
#endif
};

#endif // CROSSTIMER_H
