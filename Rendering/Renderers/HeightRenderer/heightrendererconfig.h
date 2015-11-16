#ifndef HEIGHTRENDERERCONFIG_H
#define HEIGHTRENDERERCONFIG_H

#include <GL/glew.h>
#include <QWidget>
#include <glm/glm.hpp>
#include "Rendering/Renderers/baserendererconfig.h"

namespace Ui {
class HeightRendererConfig;
}
class RModel;
class HeightRendererConfig : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit HeightRendererConfig(QWidget *parent = 0);
		virtual ~HeightRendererConfig();
		void readConfig();
		bool setBoundsFromModel();

		int axis;
		int coloring_type;
		int inverse_intensity;
		float min_x;
		float min_y;
		float min_z;
		float max_x;
		float max_y;
		float max_z;
		void setModel(RModel* model);

	private:
		Ui::HeightRendererConfig *ui;
		RModel* rmodel;
	private slots:
		void loadBoundsFromModel();

};

#endif // HEIGHTRENDERERCONFIG_H
