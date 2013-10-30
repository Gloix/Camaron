#ifndef CROSSTIMER_H
#define CROSSTIMER_H
#include <chrono>
//#include <ctime>
class CrossTimer
{
	public:
		CrossTimer();
		long getTranscurredMilliseconds();
		float getTranscurredSeconds();
		long getTranscurredNanoseconds();
	private:
		std::chrono::time_point<std::chrono::system_clock> createdAt;
		//int t;
};

#endif // CROSSTIMER_H
