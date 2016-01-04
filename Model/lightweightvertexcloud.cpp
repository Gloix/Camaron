#include "lightweightvertexcloud.h"
#include "Rendering/RModel/rmodel.h"

LightWeightVertexCloud::LightWeightVertexCloud(std::string s):
	Model(s)
{
	modelType = vis::CONSTANTS::LIGHT_WEIGHT_VERTEX_CLOUD;
	_2d = false;
}
LightWeightVertexCloud::~LightWeightVertexCloud()
{
	for( vis::LWVertex* lwVertex : vertices )
		delete lwVertex;
}

int LightWeightVertexCloud::getVerticesCount() {
	return this->verticesCount;
}
void LightWeightVertexCloud::setVerticesCount( int v ) {
	this->verticesCount = v;
}

std::vector<vis::LWVertex*>& LightWeightVertexCloud::getVertices() {
	return vertices;
}
bool LightWeightVertexCloud::is2D(){
	return _2d;
}

void LightWeightVertexCloud::set2D(bool b){
	_2d = b;
}
void LightWeightVertexCloud::clean(){
	for( vis::LWVertex* vertex : vertices )
		delete vertex;
	vertices.clear();
	Model::clean();
}
DOUBLE_DISPATCH_MODEL_DEF_LIGHT_WEIGHT(LightWeightVertexCloud)
