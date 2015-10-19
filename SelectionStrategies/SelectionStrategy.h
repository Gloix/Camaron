#ifndef SELECTIONSTRATEGY_H
#define SELECTIONSTRATEGY_H
#include <string>
#include <QWidget>
#define SELECTION_STRATEGY_BASE_WEIGHT 5.0f
namespace vis{
class Polyhedron;
class Polygon;
class Vertex;
class Element;
class Edge;
}
class VertexCloud;
class PolyhedronMesh;
class PolygonMesh;
class Selection;
class RModel;
class SelectionStrategy
{
	public:
		SelectionStrategy(std::string newname);
		virtual ~SelectionStrategy();
		//selectElementsFrom: return false if element
		virtual bool selectElement( vis::Polyhedron *, Selection* );
		virtual bool selectElement( vis::Polygon *, Selection* );
		virtual bool selectElement( vis::Vertex *, Selection* );
        virtual bool selectElement( vis::Edge *, Selection* );
		virtual void selectElement( Selection* );
		virtual bool isFullFilled( vis::Polyhedron * v );
		virtual bool isFullFilled( vis::Polygon * m );
		virtual bool isFullFilled( vis::Vertex * m );
        virtual bool isFullFilled( vis::Edge * m );
		virtual bool isFullFilled( Selection* );

		virtual bool selectElementsFrom( PolyhedronMesh*,RModel*, Selection*, bool setupPreApplied = false );
		virtual bool selectElementsFrom( PolygonMesh*,RModel*, Selection*, bool setupPreApplied = false );
		virtual bool selectElementsFrom( VertexCloud*,RModel*, Selection* , bool setupPreApplied = false);
		const char* getName() const;
		virtual bool hasConfigWidget();
		virtual void QApplicationInitiated();
		virtual QWidget* getConfigWidget();
		virtual bool applyButtonEnabled();
		virtual bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
		bool isAppliedOnSelection();
		float getWeight();
	protected:
		bool applyOnSelection;
		float weight;
	private:
		const std::string name;
};


#endif // SELECTIONSTRATEGY_H
