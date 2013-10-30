#ifndef SELECTBYID_H
#define SELECTBYID_H

#include "SelectionStrategies/SelectionStrategy.h"
class SelectByIdConfig;
namespace vis{
	class Element;
}
class SelectById: public SelectionStrategy
{
	public:
		SelectById();
		~SelectById();
		virtual bool selectElement( vis::Polyhedron *, Selection* );
		virtual bool selectElement( vis::Polygon *, Selection* );
		virtual bool selectElement( vis::Vertex *, Selection* );
		virtual bool selectElement( vis::Element *, Selection* );
		virtual bool isFullFilled( vis::Polyhedron * v );
		virtual bool isFullFilled( vis::Polygon * m );
		virtual bool isFullFilled( vis::Vertex * m );

		virtual bool hasConfigWidget();
		virtual void QApplicationInitiated();
		virtual QWidget* getConfigWidget();
		virtual bool applyButtonEnabled();
		virtual bool setup();
		virtual bool setupPreApplying(Selection*, RModel *);
	private:
		SelectByIdConfig* config;
};

#endif // SELECTBYID_H
