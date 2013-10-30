#ifndef CALCULATEPOLYGONNORMALSWORKER_H
#define CALCULATEPOLYGONNORMALSWORKER_H

#include "Utils/qtworkerbase.h"
class PolygonMesh;
class CalculatePolygonNormalsWorker: public QtWorkerBase
{
	public:
		CalculatePolygonNormalsWorker(PolygonMesh* mesh,
									  unsigned int min,
									  unsigned int max);

		void doWork();
	private:
		PolygonMesh* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // CALCULATEPOLYGONNORMALSWORKER_H
