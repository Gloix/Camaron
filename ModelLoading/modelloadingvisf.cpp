#include "modelloadingvisf.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Model/Element/Vertex.h"
#include "Utils/endianess.h"
ModelLoadingVisF::ModelLoadingVisF():
	ModelLoadingStrategy("VisF","visf")
{
}
ModelLoadingVisF::~ModelLoadingVisF()
{
	if(file.is_open())
		file.close();
}
bool ModelLoadingVisF::validate(std::string filename)
{

	file.open(filename.c_str(), std::ios::binary);
	if(!file.is_open())
		return false;
	file.seekg(0,std::ios::beg);
	unsigned char endianness;
	int n;
	file.read((char*)&endianness,sizeof(unsigned char));
	file.read((char*)&n,sizeof(int));
	file.close();
	return n<=2 && n>0 &&
			(endianness == Endianess::BIG_ENDIAN ||
			 endianness == Endianess::LITTLE_ENDIAN);

}
Model* ModelLoadingVisF::load(std::string filename){
	file.open(filename.c_str(), std::ios::binary);
	if(!file.is_open())
		return (Model*)0;
	file.seekg(0,std::ios::beg);
	file.read((char*)&fileEndianness,sizeof(unsigned char));
	int type;
	file.read((char*)&type,sizeof(int));
	if(type == vis::CONSTANTS::VERTEX_CLOUD){
		VertexCloud* newModel = new VertexCloud(filename);
		readVertices(newModel);
		file.close();
		return newModel;
	}
	if(type == vis::CONSTANTS::POLYGON_MESH){//polygon mesh
		PolygonMesh* newModel = new PolygonMesh(filename);
		readVertices(newModel);
		readPolygons(newModel);
		completeVertexPolygonRelations(newModel);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
		calculateNormalsPolygons(newModel);
		calculateNormalsVertices(newModel);
		emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
		file.close();
		return newModel;
	}
	if(type == vis::CONSTANTS::POLYHEDRON_MESH){
		PolyhedronMesh* newModel = new PolyhedronMesh(filename);
		readVertices(newModel);
		readPolygons(newModel);
		readPolyhedrons(newModel);
		completeVertexPolygonRelations(newModel);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
		completePolygonPolyhedronRelations(newModel);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYHEDRON_R);
		calculateGeoCenterPolyhedronMesh(newModel);
		emit stageComplete(ModelLoadingProgressDialog::POLYHEDRON_GEOCENTER_CALCULATED);
		calculateNormalsPolygons(newModel);
		emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
		fixSurfacePolygonsVerticesOrder(newModel);
		emit stageComplete(ModelLoadingProgressDialog::FIXED_SURFACE_POLYGONS_VERTICES_ORDER);
		calculateNormalsVertices(newModel);
		file.close();
		return newModel;

	}
	return (Model*)0;
}
bool ModelLoadingVisF::readVertices( VertexCloud* mesh){
	std::vector<float> &bounds = mesh->getBounds();
	bounds.resize(6);
	int nVertices;
	file.read((char*)&nVertices,sizeof(int));
	emit setupProgressBarForNewModel(mesh->getModelType(),nVertices,0,0);
	std::vector<vis::Vertex*>& vertices = mesh->getVertices();
	mesh->setVerticesCount(nVertices);
	vertices.resize(nVertices);
	float x,y,z;
	file.read((char*)&x,sizeof(float));
	file.read((char*)&y,sizeof(float));
	file.read((char*)&z,sizeof(float));

	vertices[0] = new vis::Vertex( 0, x, y, z );
	bounds[0] = bounds[3] = x;
	bounds[1] = bounds[4] = y;
	bounds[2] = bounds[5] = z;
	for(int i = 1;i<nVertices;i++){
		file.read((char*)&x,sizeof(float));
		file.read((char*)&y,sizeof(float));
		file.read((char*)&z,sizeof(float));
		vis::Vertex* v = new vis::Vertex(i,x,y,z);
		vertices[i] = v;
		if( bounds[0] > x )
			bounds[0] = x;
		else if( bounds[3] < x )
			bounds[3] = x;
		if( bounds[1] > y )
			bounds[1] = y;
		else if( bounds[4] < y )
			bounds[4] = y;
		if( bounds[2] > z )
			bounds[2] = z;
		else if( bounds[5] < z )
			bounds[5] = z;
		if(i%1000==0)
			emit setLoadedVertices(i);
	}
	if(fileEndianness != Endianess::findEndianness()){
		//fix endianness
		for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
			glm::vec3& currentCoords = vertices[i]->getCoords();
			currentCoords.x = Endianess::reverseBytes(currentCoords.x);
			currentCoords.y = Endianess::reverseBytes(currentCoords.y);
			currentCoords.z = Endianess::reverseBytes(currentCoords.z);
		}
	}
	emit setLoadedVertices(nVertices);
	return true;
}

bool ModelLoadingVisF::readPolygons( PolygonMesh* mesh){
	int nPolygons;
	unsigned char currentSystemEndianness = Endianess::findEndianness();
	file.read((char*)&nPolygons,sizeof(int));
	mesh->setPolygonsCount(nPolygons);
	std::vector<vis::Polygon*>& polygons = mesh->getPolygons();
	std::vector<vis::Vertex*>& vertices = mesh->getVertices();
	polygons.resize(nPolygons);
	emit setupProgressBarForNewModel(mesh->getModelType(),mesh->getVerticesCount(),nPolygons,0);
	emit setLoadedVertices(mesh->getVerticesCount());
	for(int i = 0;i<nPolygons;i++){
		int nVertices;
		file.read((char*)&nVertices,sizeof(int));
		if(fileEndianness != currentSystemEndianness )
			nVertices = Endianess::reverseBytes(nVertices);
		vis::Polygon* newPolygon;
		if(nVertices == 3)
			newPolygon = new vis::Triangle(i);
		else
			newPolygon = new vis::Polygon(i);
		std::vector<vis::Vertex*>& polygonVertices = newPolygon->getVertices();
		if(fileEndianness != currentSystemEndianness ){
			for(int j = 0;j<nVertices;j++){
				int vertexId;
				file.read((char*)&vertexId,sizeof(int));
				vertexId = Endianess::reverseBytes(vertexId);
				polygonVertices.push_back(vertices[vertexId]);
			}
		}else{//same endianness
			for(int j = 0;j<nVertices;j++){
				int vertexId;
				file.read((char*)&vertexId,sizeof(int));
				polygonVertices.push_back(vertices[vertexId]);
			}
		}
		polygons[i] = newPolygon;
		if(i%5000==0)
			emit setLoadedPolygons(i);
	}
	emit setLoadedPolygons(nPolygons);
	int hasNeighbors;
	file.read((char*)&hasNeighbors,sizeof(int));
	if(fileEndianness != currentSystemEndianness )
		hasNeighbors = Endianess::reverseBytes(hasNeighbors);
	if(hasNeighbors){
		if(fileEndianness != currentSystemEndianness ){//bad endianness
			for(int i = 0;i<nPolygons;i++){
				int nNeighbors;
				file.read((char*)&nNeighbors,sizeof(int));
				nNeighbors = Endianess::reverseBytes(nNeighbors);
				vis::Polygon* pol = polygons[i];
				std::vector<vis::Polygon*>& polygonNeighbors = pol->getNeighborPolygons();
				for(int j = 0;j<nNeighbors;j++){
					int neighborId;
					file.read((char*)&neighborId,sizeof(int));
					neighborId = Endianess::reverseBytes(neighborId);
					polygonNeighbors.push_back(polygons[neighborId]);
				}
			}

		}else{//good endianness
			for(int i = 0;i<nPolygons;i++){
				int nNeighbors;
				file.read((char*)&nNeighbors,sizeof(int));
				vis::Polygon* pol = polygons[i];
				std::vector<vis::Polygon*>& polygonNeighbors = pol->getNeighborPolygons();
				for(int j = 0;j<nNeighbors;j++){
					int neighborId;
					file.read((char*)&neighborId,sizeof(int));
					polygonNeighbors.push_back(polygons[neighborId]);
				}
			}
		}
	}else{
		completePolygonPolygonRelations(mesh);
	}
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
	return true;
}
bool ModelLoadingVisF::readPolyhedrons( PolyhedronMesh* mesh){
	unsigned char currentSystemEndianness = Endianess::findEndianness();
	int nPolyhedrons;
	file.read((char*)&nPolyhedrons,sizeof(int));
	if(fileEndianness != currentSystemEndianness )//bad endianness
		nPolyhedrons = Endianess::reverseBytes(nPolyhedrons);
	mesh->setPolyhedronsCount(nPolyhedrons);
	std::vector<vis::Polygon*>& polygons = mesh->getPolygons();
	std::vector<vis::Polyhedron*>& polyhedrons = mesh->getPolyhedrons();
	polyhedrons.resize(nPolyhedrons);
	emit setupProgressBarForNewModel(mesh->getModelType(),mesh->getVerticesCount(),
									 mesh->getPolygonsCount(),nPolyhedrons);
	emit setLoadedVertices(mesh->getVerticesCount());
	emit setLoadedPolygons(mesh->getPolygonsCount());
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
	if(fileEndianness != currentSystemEndianness ){//bad endianness
		for(int i = 0;i<nPolyhedrons;i++){
			int nPolygons;
			file.read((char*)&nPolygons,sizeof(int));
			nPolygons = Endianess::reverseBytes(nPolygons);
			vis::Polyhedron* newPolyhedron = new vis::Polyhedron(i);
			std::vector<vis::Polygon*>& polyhedronPolygons = newPolyhedron->getPolyhedronPolygons();
			for(int j = 0;j<nPolygons;j++){
				int polygonId;
				file.read((char*)&polygonId,sizeof(int));
				polygonId = Endianess::reverseBytes(polygonId);
				polyhedronPolygons.push_back(polygons[polygonId]);
			}
			polyhedrons[i] = newPolyhedron;
			if(i%5000==0)
				emit setLoadedPolyhedrons(i);
		}
	}else{
		for(int i = 0;i<nPolyhedrons;i++){
			int nPolygons;
			file.read((char*)&nPolygons,sizeof(int));
			vis::Polyhedron* newPolyhedron = new vis::Polyhedron(i);
			std::vector<vis::Polygon*>& polyhedronPolygons = newPolyhedron->getPolyhedronPolygons();
			for(int j = 0;j<nPolygons;j++){
				int polygonId;
				file.read((char*)&polygonId,sizeof(int));
				polyhedronPolygons.push_back(polygons[polygonId]);
			}
			polyhedrons[i] = newPolyhedron;
			if(i%5000==0)
				emit setLoadedPolyhedrons(i);
		}
	}

	emit setLoadedPolyhedrons(nPolyhedrons);
	return true;
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingVisF);
