#ifndef CONVEXGEOMETRYINTERSECTIONRENDERERCONFIG_H
#define CONVEXGEOMETRYINTERSECTIONRENDERERCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"
namespace Ui {
class ConvexGeometryIntersectionRendererConfig;
}
class MainRendererConfig;
class ConvexGeometryIntersectionRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit ConvexGeometryIntersectionRendererConfig(QWidget *parent = 0);
		~ConvexGeometryIntersectionRendererConfig();
		void readConfig();
		void loadedModelIsLightWeight(bool isLightWeight);
		MainRendererConfig* mainRendererConfig;

		std::string geometryText;
		bool geometryTextChanged;
		bool drawNormal;
		bool followModel;
		bool changedFollowModel;
		float normalLength;
		glm::vec4 convexGeometryColor;
		glm::vec4 convexGeometryNormalColor;
		glm::vec4 borderCutColor;
		glm::vec4 outsideColor;
		glm::vec4 outsideSelectedColor;
		bool invertIntersection;
		bool resetPosition;
		bool drawOutsideElements;
	public slots:
		void getFromFile();
		void saveToFile();
		void changeTextToPolygonsFormat();
		void changeTextToNormalsFormat();
		void changeTextToSphereFormat();
		void textChangedChangeBool();
		
	private:
		Ui::ConvexGeometryIntersectionRendererConfig *ui;
};

#endif // CONVEXGEOMETRYINTERSECTIONRENDERERCONFIG_H
