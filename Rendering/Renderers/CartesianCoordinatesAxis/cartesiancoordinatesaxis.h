#ifndef CARTESIANCOORDINATESAXIS_H
#define CARTESIANCOORDINATESAXIS_H
#include <GL/glew.h>
#include "Rendering/Renderer.h"
#include "Rendering/Renderers/CartesianCoordinatesAxis/cartesiancoordinatesaxisconfig.h"
class CartesianCoordinatesAxis:public Renderer
{
	public:
		CartesianCoordinatesAxis();
		virtual ~CartesianCoordinatesAxis();
		virtual void glewIsReadyRenderer();
		void draw(RModel*);
		QWidget* getRendererConfigWidget();
		bool hasRendererConfigWidget();
		void applyConfigChanges(RModel* rmodel = (RModel*)0);
		bool rmodelChanged(RModel* rmodel);
	private:
		CartesianCoordinatesAxisConfig* config;
		GLuint theProgram;
		GLuint coordinatesBoxPointer;
		GLuint pointsPointer;
		std::vector<glm::vec3> coordinatesBox;
		std::vector<glm::vec3> points;
};

#endif // CARTESIANCOORDINATESAXIS_H
