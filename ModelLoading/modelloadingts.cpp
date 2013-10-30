#include "modelloadingts.h"
#include "Utils/hashingtree.h"
#include "Utils/fileutils.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/triangle.h"
#include <iostream>
ModelLoadingTs::ModelLoadingTs():
	ModelLoadingStrategy("ts","ts")
{
}
ModelLoadingTs::~ModelLoadingTs(){}
bool ModelLoadingTs::validate(std::string)
{
	return true;
}
Model* ModelLoadingTs::load(std::string filename)
{
	fileSize = FileUtils::getFileSize(filename);
	FILE* file = fopen( filename.c_str(), "r" );
	fileBuffer = (char*)malloc(fileSize+1L);
	long readed = fread(fileBuffer,1,fileSize,file);
	fclose(file);
	fileBuffer[fileSize] = '\0';
	scanner.reset(readed);
	PolyhedronMesh* loaded = readHeader(filename);
	if( readBody( loaded ) ){
		this->completeVertexPolygonRelations(loaded);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
		completePolygonPolygonRelations(loaded);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
		completePolygonPolyhedronRelations(loaded);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYHEDRON_R);
		calculateGeoCenterPolyhedronMesh(loaded);
		emit stageComplete(ModelLoadingProgressDialog::POLYHEDRON_GEOCENTER_CALCULATED);
		calculateNormalsPolygons(loaded);
		emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
		fixSurfacePolygonsVerticesOrder(loaded);
		emit stageComplete(ModelLoadingProgressDialog::FIXED_SURFACE_POLYGONS_VERTICES_ORDER);
		calculateNormalsVertices(loaded);
		delete[] fileBuffer;
		fileSize = 0;
		return ( Model* )loaded;
	}
	delete loaded;
	delete[] fileBuffer;
	fileSize = 0;
	return ( Model* )NULL;
}
PolyhedronMesh* ModelLoadingTs::readHeader(std::string filename)
{
	PolyhedronMesh* polyhedronMesh = new PolyhedronMesh(filename);
	int nvertex = 0;
	int npolyhedron = 0;
	//read number of points
	scanner.readInt(fileBuffer,&nvertex );
	//read number of faces}
	scanner.readInt(fileBuffer,&npolyhedron );
	polyhedronMesh->setPolyhedronsCount(npolyhedron);
	polyhedronMesh->setVerticesCount( nvertex );
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYHEDRON_MESH,nvertex,npolyhedron,npolyhedron);
	//scanner.readInt(fileBuffer,&nf );//discard number of edges
	scanner.skipToNextLine(fileBuffer);
	return polyhedronMesh;
}

bool ModelLoadingTs::readPolyhedrons( PolyhedronMesh* mesh ) {
	int polygonIndex = 0;
	std::vector<vis::Polygon*>& triangles = mesh->getPolygons();
	std::vector<vis::Polyhedron*>& tetrahedrons = mesh->getPolyhedrons();
	std::vector<vis::Vertex*>& vertices = mesh->getVertices();
	//vertices.reserve(numberOfNodes);
	vis::ut::HashingTree<int,vis::Polygon> hashingTree;
	int v[4];
	for(int i = 0; i<mesh->getPolyhedronsCount();i++){
		vis::Polyhedron* current = new vis::Polyhedron(i);
		std::vector<vis::Polygon*>& currentPolyhedronTriangles = current->getPolyhedronPolygons();
		scanner.readInt(fileBuffer,&v[0],true);
		scanner.readInt(fileBuffer,&v[1]);
		scanner.readInt(fileBuffer,&v[2]);
		scanner.readInt(fileBuffer,&v[3]);
		for(int j = 0;j<4;j++){
			std::vector<int> triangleVertices = {v[j],v[(j+1)%4],v[(j+2)%4]};
			vis::Polygon* polygon = hashingTree.getRegistry(triangleVertices);
			if(!polygon){
				polygon = new vis::Triangle(polygonIndex++);
				hashingTree.registerValue(triangleVertices,polygon);
				triangles.push_back(polygon);
				std::vector<vis::Vertex*>& currentPolygonVertices = polygon->getVertices();
				currentPolygonVertices.push_back(vertices[triangleVertices[0]]);
				currentPolygonVertices.push_back(vertices[triangleVertices[1]]);
				currentPolygonVertices.push_back(vertices[triangleVertices[2]]);
			}
			currentPolyhedronTriangles.push_back(polygon);
		}
		tetrahedrons.push_back(current);
		scanner.skipToNextLine(fileBuffer);
		if(i%5000==0){
			emit setLoadedPolyhedrons(i);
			emit setLoadedPolygons(i);
		}
	}
	mesh->setPolygonsCount(polygonIndex);
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYHEDRON_MESH,mesh->getVerticesCount(),
									 mesh->getPolygonsCount(),mesh->getPolyhedronsCount());
	emit setLoadedVertices(mesh->getVerticesCount());
	emit setLoadedPolygons(mesh->getPolygonsCount());
	emit setLoadedPolyhedrons(mesh->getPolyhedronsCount());
	return true;
}
bool ModelLoadingTs::readVertices( PolyhedronMesh* pol ) {
	std::vector<vis::Vertex*> &v = pol->getVertices();
	std::vector<float> &bounds = pol->getBounds();
	v.clear();
	v.reserve( pol->getVerticesCount() );
	bounds.resize( 6 );
	float x = 0.0f;
	scanner.readFloat(fileBuffer, &x );
	float y = 0.0f;
	scanner.readFloat(fileBuffer, &y );
	float z =0.0f;
	scanner.readFloat(fileBuffer, &z );
	v.push_back( new vis::Vertex( 0, x, y, z ) );
	bounds[0] = bounds[3] = x;
	bounds[1] = bounds[4] = y;
	bounds[2] = bounds[5] = z;
	scanner.skipToNextLine(fileBuffer);

	for( int i = 1; i < pol->getVerticesCount(); i++ ) {
		scanner.readFloat(fileBuffer, &x );
		scanner.readFloat(fileBuffer, &y );
		scanner.readFloat(fileBuffer, &z );
		v.push_back( new vis::Vertex( i, x, y, z ) );
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
		scanner.skipToNextLine(fileBuffer);
		if(i%1000==0)
			emit setLoadedVertices(i);
	}
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingOff] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
	emit setLoadedVertices(pol->getVerticesCount());
	return true;
}
bool ModelLoadingTs::readBody( PolyhedronMesh* pol )
{
	if( readVertices( pol ) && readPolyhedrons( pol ) )
		return true;
	return false;
}
#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingTs);
