#ifndef CALCULATEPOLYHEDRONGEOCENTERWORKER_H
#define CALCULATEPOLYHEDRONGEOCENTERWORKER_H

#include "Utils/qtworkerbase.h"
class PolyhedronMesh;
class CalculatePolyhedronGeocenterWorker: public QtWorkerBase
{
	public:
		CalculatePolyhedronGeocenterWorker(PolyhedronMesh* mesh,
										   unsigned int min,
										   unsigned int max);

		void doWork();
	private:
		PolyhedronMesh* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // CALCULATEPOLYHEDRONGEOCENTERWORKER_H
