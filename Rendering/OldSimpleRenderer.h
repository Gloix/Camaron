#ifndef OLDSIMPLERENDERER_H
#define OLDSIMPLERENDERER_H

#include "Rendering/Renderer.h"
class OldSimpleRenderer: public Renderer{
	public:
		OldSimpleRenderer();
		virtual ~OldSimpleRenderer();
		virtual void setUpDrawing();
		void draw(RModel*);
};

#endif // OLDSIMPLERENDERER_H
