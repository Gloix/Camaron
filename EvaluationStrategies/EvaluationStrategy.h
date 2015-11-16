#ifndef EVALUATIONSTRATEGY_H
#define EVALUATIONSTRATEGY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <QWidget>
#define VERTEX_CRITERIA_WEIGHT_BASE 1.0f
#define POLYGON_CRITERIA_WEIGHT_BASE 100.0f
#define TRIANGLE_CRITERIA_WEIGHT_BASE 150.0f
#define POLYHEDRON_CRITERIA_WEIGHT_BASE 200.0f
namespace vis{
class Vertex;
class Polyhedron;
class Polygon;
class Triangle;
class Edge;
}
class VertexCloud;
class PolyhedronMesh;
class PolygonMesh;
class EvaluationStrategy
{
	public:
		EvaluationStrategy(std::string newname,
						   std::string newPropertyName,
						   unsigned char idn = 0);
		EvaluationStrategy(std::string newname,std::string newdescription,
						   std::string newPropertyName,
						   unsigned char idn = 0);
		virtual ~EvaluationStrategy();
		void setId(unsigned char idn);
		std::string getDescription();
		virtual float value( vis::Vertex* v );
		virtual float value( vis::Polygon* m );
		virtual float value( vis::Triangle* m );
		virtual float value( vis::Polyhedron* m );
		virtual float value( vis::Edge* m );
		const char *getName( );
		const char* getPropertyName( );
		virtual bool isFullFilled( vis::Vertex* v );
		virtual bool isFullFilled( vis::Polygon* m );
		virtual bool isFullFilled( vis::Triangle* m );
		virtual bool isFullFilled( vis::Polyhedron* m );
		virtual bool isFullFilled( vis::Edge* m );
		float getWeight();
		void setWeight(float);

		virtual void setup( PolyhedronMesh* );
		virtual void setup( PolygonMesh* );
		virtual void setup( VertexCloud* );


		virtual void evaluateElementsFrom( PolyhedronMesh*, std::vector<float>& values );
		virtual void evaluateElementsFrom( PolygonMesh*, std::vector<float>& values );
		virtual void evaluateElementsFrom( VertexCloud*, std::vector<float>& values );
		//stadistics
		void setTotalElementsCount(int);
		int getTotalElementsCount();
		int getCalculatedValuesCount();
		int getValuesCountInRange(float from, float to);
		float getMinValue();
		float getMaxValue();
		bool hasStatics();
		//could not be applied
		virtual float getNullValue() = 0;
		//Description QWidget
		virtual bool hasDescriptionQWidget();
		virtual QWidget* getEvaluationStrategyDescriptionQWidget();
		virtual void QApplicationInitiatedEv();
		bool hasQIcon();
		QIcon* getEvaluationStrategyQIcon();
	protected:
		const std::string description;
		const std::string name;
		const std::string propertyName;
		void addNewValue(float);
		unsigned char id;
		bool saveStatics;
		std::vector<float> values;
		float minValue;
		float maxValue;
		int totalElementsCount;
		bool valuesNeedSorting;
		float weight;
		QIcon* qicon;
	private:
		void initVariables(int idn);
};

#endif // EVALUATIONSTRATEGY_H
