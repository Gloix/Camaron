#ifndef PHONGPERPIXEL_H
#define PHONGPERPIXEL_H
#include "Rendering/Renderer.h"
#include <GL/glew.h>
class ShadingEffectsRenderConfig;
class PhongPerPixel:public Renderer
{
	public:
		PhongPerPixel();
		virtual ~PhongPerPixel();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		QWidget* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel* rmodel = (RModel*)0);
	private:
		ShadingEffectsRenderConfig* config;
		GLuint theProgram;
};
#endif // PHONGPERPIXEL_H
