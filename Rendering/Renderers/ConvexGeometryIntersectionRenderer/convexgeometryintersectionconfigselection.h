#ifndef CONVEXGEOMETRYINTERSECTIONCONFIGSELECTION_H
#define CONVEXGEOMETRYINTERSECTIONCONFIGSELECTION_H

#include <QWidget>
#include "Rendering/Renderers/baserendererconfig.h"

namespace Ui {
class ConvexGeometryIntersectionConfigSelection;
}

class ConvexGeometryIntersectionConfigSelection : public BaseRendererConfig
{
		Q_OBJECT
		
	public:
		explicit ConvexGeometryIntersectionConfigSelection(QWidget *parent = 0);
		~ConvexGeometryIntersectionConfigSelection();
		void readConfig();
		static const int NEW_SELECTION = 0;
		static const int JOIN_SELECTION = 1;
		static const int INTERSECT_SELECTION = 2;
		static const int INVERT_SELECTION = 3;
		int elementsType;//polygon, vertex or polyhedron
		int selectionType;
		
	private:
		Ui::ConvexGeometryIntersectionConfigSelection *ui;
};

#endif // CONVEXGEOMETRYINTERSECTIONCONFIGSELECTION_H
