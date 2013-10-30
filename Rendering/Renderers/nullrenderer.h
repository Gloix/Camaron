#ifndef NULLRENDERER_H
#define NULLRENDERER_H
#include "Rendering/Renderer.h"
class NullRenderer:public Renderer
{
	public:
		NullRenderer();
		virtual ~NullRenderer();
		bool rmodelChanged(RModel* rmodel);
		bool isValidAsSecondaryRenderer();
};

#endif // NULLRENDERER_H
