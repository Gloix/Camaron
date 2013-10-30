#ifndef ELEMENT_H
#define ELEMENT_H
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <Utils/lowmemoryhash.hpp>

#define DOUBLE_DISPATCH_ELEMENT_DEC0 \
	virtual bool fullFillsEvaluationStrategy(EvaluationStrategy*) = 0;\
	virtual float evaluateUsingEvaluationStrategy(EvaluationStrategy*) = 0;\
	virtual bool applySelectionStrategyDD(SelectionStrategy*, Selection*) = 0;
#define DOUBLE_DISPATCH_ELEMENT_DEC \
	virtual bool fullFillsEvaluationStrategy(EvaluationStrategy*);\
	virtual float evaluateUsingEvaluationStrategy(EvaluationStrategy*);\
	virtual bool applySelectionStrategyDD(SelectionStrategy*, Selection*);
#define DOUBLE_DISPATCH_ELEMENT_DEF(x) \
	bool x::fullFillsEvaluationStrategy(EvaluationStrategy* e){\
	return e->isFullFilled(this);\
	}\
	float x::evaluateUsingEvaluationStrategy(EvaluationStrategy* e){\
	return e->value(this);\
	}\
	bool x::applySelectionStrategyDD(SelectionStrategy* stra, Selection* sel){\
	return (stra->isFullFilled(this))?stra->selectElement(this,sel):false;\
	}



class SelectionStrategy;
class EvaluationStrategy;
class Selection;
namespace vis{

class Element
{
	public:
		Element(int);
		virtual ~Element();
		int getId();
		bool isSelected();
		void setSelected(bool);
		virtual bool isAtSurface() = 0;
		virtual glm::vec3 getGeometricCenter() = 0;

		//properties
		LowMemoryHash<unsigned char, float>& getProperties();
		bool hasProperty(int key);
		void addProperty(int key, float value);
		float getProperty(int key);

		//rmodel
		void addRmodelPositions(int pos);
		std::vector<int>& getRmodelPositions();

		//Double Dispatch
		DOUBLE_DISPATCH_ELEMENT_DEC0
	protected:
		int id;
		bool selected;
	private:
		std::vector<int> rmodelPositions;
		LowMemoryHash<unsigned char, float> properties;
};
}
#endif // ELEMENT_H
