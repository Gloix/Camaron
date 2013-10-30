#ifndef CALCULATEVERTEXNORMALSLIGHTWEIGHTMODELWORKER_H
#define CALCULATEVERTEXNORMALSLIGHTWEIGHTMODELWORKER_H

#include "Utils/qtworkerbase.h"
class LightWeightPolygonMesh;
class CalculateVertexNormalsLightWeightModelWorker: public QtWorkerBase
{
	public:
		CalculateVertexNormalsLightWeightModelWorker(LightWeightPolygonMesh* mesh,
													 unsigned int min,
													 unsigned int max);

		void doWork();
	private:
		LightWeightPolygonMesh* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // CALCULATEVERTEXNORMALSLIGHTWEIGHTMODELWORKER_H
