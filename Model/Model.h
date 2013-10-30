#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Common/Constants.h"


#define DOUBLE_DISPATCH_MODEL_DEC0 \
	virtual bool selectUsingSelectionStrategy( SelectionStrategy*,RModel*, Selection* ) = 0;\
	virtual void evaluateUsingEvaluationStrategy( EvaluationStrategy*, std::vector<float>& ) = 0;\
	virtual void setupEvaluationStrategy( EvaluationStrategy* ) = 0;\
	virtual void loadModelDataIntoRModel( RModel* ) = 0;\
	virtual bool exportModelDD(ModelExportStrategy*, std::string) = 0;\
	virtual void fillModelStatsDD(ModelGeneralStaticsCollector*) = 0;
#define DOUBLE_DISPATCH_MODEL_DEC \
	virtual bool selectUsingSelectionStrategy( SelectionStrategy*,RModel*, Selection* );\
	virtual void evaluateUsingEvaluationStrategy( EvaluationStrategy*, std::vector<float>& );\
	virtual void setupEvaluationStrategy( EvaluationStrategy* );\
	virtual void loadModelDataIntoRModel( RModel* );\
	virtual bool exportModelDD(ModelExportStrategy*, std::string);\
	virtual void fillModelStatsDD(ModelGeneralStaticsCollector*);
#define DOUBLE_DISPATCH_MODEL_DEF(x) \
	bool x::selectUsingSelectionStrategy( SelectionStrategy* strategy,RModel* rmodel, Selection* selection ) {\
		return strategy->selectElementsFrom(this,rmodel,selection);\
	}\
	void x::evaluateUsingEvaluationStrategy( EvaluationStrategy* strategy, std::vector<float>& values ) {\
		strategy->evaluateElementsFrom(this,values);\
	}\
	void x::setupEvaluationStrategy( EvaluationStrategy* strategy){\
		strategy->setup(this);\
	}\
	void x::loadModelDataIntoRModel( RModel* rmodel){\
		rmodel->loadRModelData(this);\
	}\
	bool x::exportModelDD( ModelExportStrategy* strategy, std::string filename){\
		return strategy->exportModel(this,filename);\
	}\
	void x::fillModelStatsDD(ModelGeneralStaticsCollector* m){\
		return m->fillModelStatics(this);\
	}
#define DOUBLE_DISPATCH_MODEL_DEF_LIGHT_WEIGHT(x) \
	bool x::selectUsingSelectionStrategy( SelectionStrategy*,RModel*, Selection* ) {\
		return false;\
	}\
	void x::evaluateUsingEvaluationStrategy( EvaluationStrategy*, std::vector<float>& ) {\
		return;\
	}\
	void x::setupEvaluationStrategy( EvaluationStrategy* ){\
		return;\
	}\
	void x::loadModelDataIntoRModel( RModel* rmodel){\
		rmodel->loadRModelData(this);\
	}\
	bool x::exportModelDD( ModelExportStrategy*, std::string){\
		return false;\
	}\
	void x::fillModelStatsDD(ModelGeneralStaticsCollector*){\
		return;\
	}
class SelectionStrategy;
class Selection;
class EvaluationStrategy;
class ModelExportStrategy;
class RModel;
class ModelGeneralStaticsCollector;
class Model
{
	public:
		Model(std::string filename);
		virtual ~Model();
		std::vector<float>& getBounds();
		DOUBLE_DISPATCH_MODEL_DEC0
		virtual void evaluateUsingEvaluationStrategy( EvaluationStrategy* );
		int getModelType();
		std::string getFilename();
		bool isLightWeight();
		virtual void clean();
	protected:
		std::vector<float> bounds; //{min x, min y, min z, max x, max y, max z}
		int modelType;
	private:
		std::string filename;
};

#endif // MODEL_H
