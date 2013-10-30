#ifndef PROPERTYVALUESRENDERER_H
#define PROPERTYVALUESRENDERER_H

#include <GL/glew.h>
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/PropertyValuesRenderer/propertyvaluesrendererconfig.h"
class PropertyValuesRenderer: public Renderer
{
	public:
		PropertyValuesRenderer();
		virtual ~PropertyValuesRenderer();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		QWidget* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel* rmodel = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);

	private:
		PropertyValuesRendererConfig* config;
		GLuint theProgram;
		GLuint vertexPropertyValueBuffer;
		std::vector<GLfloat> propertyData;
		void createVertexPropertyData(RModel*);
};

#endif // PROPERTYVALUESRENDERER_H
