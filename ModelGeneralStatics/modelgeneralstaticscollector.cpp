#include "modelgeneralstaticscollector.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Polygon.h"
#include <unordered_map>
ModelGeneralStaticsCollector::ModelGeneralStaticsCollector()
{
}

void ModelGeneralStaticsCollector::fillModelStatics(VertexCloud* v){
	//vertex statics
	ModelStaticsItem itemVertex("Vertex Statistics");
	itemVertex.addStat("Total Vertex Count:",(int)v->getVertices().size());
	modelStatics.push_back(itemVertex);
	//bounds
	std::vector<float>& bounds = v->getBounds();
	ModelStaticsItem item("Model Bounds");

	item.addStat("Min X: ",bounds[0]);
	item.addStat("Min Y: ",bounds[1]);
	item.addStat("Min Z: ",bounds[2]);
	item.addStat("Max X: ",bounds[3]);
	item.addStat("Max Y: ",bounds[4]);
	item.addStat("Max Z: ",bounds[5]);
	modelStatics.push_back(item);
}

void ModelGeneralStaticsCollector::fillModelStatics(PolygonMesh* mesh){
	std::unordered_map<int,int> verticesCount;
	std::vector<vis::Polygon*>& polygons = mesh->getPolygons();
	for(vis::Polygon* polygon : polygons)
		verticesCount[polygon->getVertices().size()]++;

	ModelStaticsItem item("Polygon Statistics");
	typedef std::unordered_map<int,int>::const_iterator map_it;
	item.addStat("Total Polygons Count:",(int)mesh->getPolygons().size());
	for(map_it i = verticesCount.begin();i!=verticesCount.end();i++){
		std::stringstream ss;
		ss << i->first;
		ss << " vertices:";
		item.addStat(ss.str(),(int)i->second);
	}
	modelStatics.push_back(item);

	fillModelStatics((VertexCloud*)mesh);
}

void ModelGeneralStaticsCollector::fillModelStatics(PolyhedronMesh* mesh){
	std::unordered_map<int,int> facesCount;
	std::vector<vis::Polyhedron*>& polyhedrons = mesh->getPolyhedrons();
	for(vis::Polyhedron* polyhedron : polyhedrons)
		facesCount[polyhedron->getPolyhedronPolygons().size()]++;

	ModelStaticsItem item("Polyhedron Statistics");
	item.addStat("Total Polyhedrons Count:",(int)mesh->getPolyhedrons().size());
	typedef std::unordered_map<int,int>::const_iterator map_it;
	for(map_it i = facesCount.begin();i!=facesCount.end();i++){
		std::stringstream ss;
		ss << i->first;
		ss << " faces:";
		item.addStat(ss.str(),(int)i->second);
	}
	modelStatics.push_back(item);
	fillModelStatics((PolygonMesh*)mesh);
}
std::vector<ModelStaticsItem>& ModelGeneralStaticsCollector::getModelStatics(){
	return modelStatics;
}

ModelStaticsItem* ModelGeneralStaticsCollector::findItem(std::string title){
	typedef std::vector<ModelStaticsItem*>::size_type it_t;
	for(it_t i = 0;i<modelStatics.size();i++)
		if(title.compare(modelStatics[i].itemTitle)==0)
			return &modelStatics[i];
	return (ModelStaticsItem*)0;
}

void ModelGeneralStaticsCollector::clean(){
	modelStatics.clear();
}
