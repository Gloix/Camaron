#ifndef RENDERINGLOCK_H
#define RENDERINGLOCK_H

#define WORKING_IN_WINDOWS
#ifndef __MINGW32__
	#undef WORKING_IN_WINDOWS
#endif
// Use Window or Posix
#ifdef WORKING_IN_WINDOWS
	 #include <windows.h>
#else
	 #ifndef POSIX
		  #warning POSIX will be used (but you did not define it)
	 #endif
	 #include <pthread.h>
#endif

#include <QSemaphore>

class RenderingLock
{
	public:
		RenderingLock();
		~RenderingLock();
		bool needsRendering();
		void setRenderingNeed(bool);
	private:
		bool needRendering;
		QSemaphore qsemaphore;

#ifdef WORKING_IN_WINDOWS
	 CRITICAL_SECTION _mutex; /**< Window mutex */
#else
	 pthread_mutex_t _mutex; /**< posix mutex */
#endif
};

#endif // RENDERINGLOCK_H
