#ifndef CALCULATEVERTEXNORMALSWORKER_H
#define CALCULATEVERTEXNORMALSWORKER_H

#include "Utils/qtworkerbase.h"
class PolygonMesh;
class CalculateVertexNormalsWorker: public QtWorkerBase
{
	public:
		CalculateVertexNormalsWorker(PolygonMesh* mesh,
									 unsigned int min,
									 unsigned int max);

		void doWork();
	private:
		PolygonMesh* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // CALCULATEVERTEXNORMALSWORKER_H
