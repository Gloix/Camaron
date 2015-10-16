#ifndef CARTESIANCOORDINATESAXISCONFIG_H
#define CARTESIANCOORDINATESAXISCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class CartesianCoordinatesAxisConfig;
}

class CartesianCoordinatesAxisConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit CartesianCoordinatesAxisConfig(QWidget *parent = 0);
		~CartesianCoordinatesAxisConfig();
		void readConfig();
		//config variables
		glm::vec3 boundingBoxColor;
		glm::vec3 xAxisColor;
		glm::vec3 yAxisColor;
		glm::vec3 zAxisColor;
		glm::vec3 originPointColor;
		glm::vec3 extremePointsColor;
		float extremePointSize;
		float originPointSize;
		float boundingBoxLineWidth;
		float axisLineWidth;
		bool drawAxix;
		bool drawBoundingBox;
		bool drawExtremePoints;
		bool drawOriginPoint;
	private:
		Ui::CartesianCoordinatesAxisConfig *ui;

};

#endif // CARTESIANCOORDINATESAXISCONFIG_H
