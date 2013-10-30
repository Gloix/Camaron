#ifndef SELECTION_H
#define SELECTION_H
#include <unordered_map>
namespace vis{
class Element;
class Polyhedron;
}
class EvaluationStrategy;
class RModel;
class Selection
{
	public:
		Selection();
		virtual ~Selection();
		std::unordered_map<int, vis::Element*>& getSelectedElements();
		void clearSelectedElements();
		bool addSelectedElement( vis::Element* );
		bool removeSelectedElement( vis::Element* );
		bool removeSelectedElement( vis::Polyhedron* );
		void evaluateUsingEvaluationStrategy( EvaluationStrategy* strategy );
		void setSelectionType(int);
		int getSelectionType();
		void setRModel(RModel*);
		void reset();
		static const int NOT_INITIALIZED = -1;
	protected:
	private:
		std::unordered_map<int, vis::Element*> selectedElements;
		int selectionType;
		RModel* rmodel;

};

#endif // SELECTION_H
