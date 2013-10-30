#include "polyhedronvolume.h"
#include "Model/Element/Polyhedron.h"
#include <QIcon>
PolyhedronVolume::PolyhedronVolume():
	EvaluationStrategy("Volume", "Volume")
{
    //weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.5f;
    weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.57f;
}

PolyhedronVolume::~PolyhedronVolume()
{
}
float PolyhedronVolume::value( vis::Polyhedron* m ) {
	if(m->hasProperty(this->id))
		return m->getProperty(this->id);
	float volume = m->getVolume();
	m->addProperty(this->id, volume);
	this->addNewValue(volume);
	return volume;
}
bool PolyhedronVolume::isFullFilled( vis::Polyhedron* ){
	return true;
}
float PolyhedronVolume::getNullValue(){
	return -1.0f;
}
void PolyhedronVolume::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolyhedronVolume);
