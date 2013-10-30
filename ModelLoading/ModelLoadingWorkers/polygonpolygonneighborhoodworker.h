#ifndef POLYGONPOLYGONNEIGHBORHOODWORKER_H
#define POLYGONPOLYGONNEIGHBORHOODWORKER_H
#include "Utils/qtworkerbase.h"
class PolygonMesh;
class PolygonPolygonNeighborhoodWorker: public QtWorkerBase
{
	public:
		PolygonPolygonNeighborhoodWorker(PolygonMesh* mesh,
										 unsigned int min,
										 unsigned int max);

		void doWork();
	private:
		PolygonMesh* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // POLYGONPOLYGONNEIGHBORHOODWORKER_H
