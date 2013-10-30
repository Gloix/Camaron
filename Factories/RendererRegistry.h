#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H
#include "RegistryTemplate.h"
#include "Rendering/Renderer.h"
#define REGISTER_RENDERER(x) \
	bool x##_dummy_var_render  = \
	RendererRegistry::getSingletonInstance()->\
	insertRenderer(new x())
#define REGISTER_RENDERER_INSTANCE(x) \
	bool x##_dummy_var_render_inst  = \
	RendererRegistry::getSingletonInstance()->\
	insertRenderer(x)
class RendererRegistry:public RegistryTemplate<int,Renderer>
{
	public:
		virtual ~RendererRegistry();
		Renderer* getByName(std::string);
		static RendererRegistry* getSingletonInstance();
		static void deleteSingletonInstance();
		bool insertRenderer(Renderer*);
		void glewIsReady();
	private:
		RendererRegistry();
		bool insertNextRenderer(Renderer*);
		int nextKey;
		static RendererRegistry* instance;
		std::vector<Renderer*> toInsert;
};

#endif // RENDERERFACTORY_H
