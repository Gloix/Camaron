#include "nullrenderer.h"

NullRenderer::NullRenderer():Renderer("No Renderer"){
	rendererWeight = RENDERER_WEIGHT_BASE+2000.0f; // last in the list
}
NullRenderer::~NullRenderer()
{
}

bool NullRenderer::rmodelChanged(RModel*){
	return true;
}
bool NullRenderer::isValidAsSecondaryRenderer(){
	return false;
}

#include "Factories/RendererRegistry.h"
REGISTER_RENDERER(NullRenderer);
