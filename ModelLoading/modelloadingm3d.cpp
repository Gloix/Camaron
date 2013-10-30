#include "modelloadingm3d.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Model/PolyhedronMesh.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
ModelLoadingM3D::ModelLoadingM3D():ModelLoadingStrategy("M3D", "m3d")
{
	fileBuffer = (char*)0;
	fileSize = 0;
}

ModelLoadingM3D::~ModelLoadingM3D(){}


bool ModelLoadingM3D::validate(std::string filename)
{

	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	if(fileSize==0)
		return false;
	scanner.reset(fileSize);
	char word [256];
	bool valid = false;
	while(scanner.readString(fileBuffer, word,true)){
		if( !strcmp( word, "[Nodes,\0" ) ){
			scanner.readString(fileBuffer, word,true);
			valid = !strcmp(word,"ARRAY1<STRING>]\0");
			break;
		}
	}
	delete[] fileBuffer;
	return valid;
}
Model* ModelLoadingM3D::load(std::string filename){
	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	scanner.reset(fileSize);

	PolyhedronMesh* loaded = new PolyhedronMesh(filename);
	try{
		if( readBody( loaded ) ){
			completeVertexPolygonRelations(loaded);
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
	}catch(std::bad_alloc &ba){
		delete loaded;
		delete[] fileBuffer;
		fileSize = 0;
		throw std::bad_alloc();
	}

	delete loaded;
	delete[] fileBuffer;
	fileSize = 0;
	return ( Model* )NULL;
}
vis::Polygon* ModelLoadingM3D::createPolygon(std::vector<int>& indices,
											 std::vector<vis::Vertex*>& vertices,
											 int polygonIndex,
											 vis::ut::HashingTree<int,vis::Polygon>& hashingTree){
	vis::Polygon* newPolygon;
	if(indices.size()==3){
		newPolygon = new vis::Triangle(polygonIndex);
	}else
		newPolygon = new vis::Polygon(polygonIndex);
	hashingTree.registerValue(indices,newPolygon);
	std::vector<vis::Vertex*>& currentPolygonVertices = newPolygon->getVertices();
	for(std::vector<int>::size_type i = 0;i<indices.size();i++)
		currentPolygonVertices.push_back(vertices[indices[i]]);
	return newPolygon;
}
void ModelLoadingM3D::readM3DPolyhedron(int nVertices,
										std::vector<std::vector<int> >& verticesIndicesPerFace,
										PolyhedronMesh* pol,
										vis::ut::HashingTree<int,vis::Polygon>& hashingTree){
	std::vector<vis::Vertex*>& vertices = pol->getVertices();
	std::vector<int> verticesIndices;
	verticesIndices.resize(nVertices);
	for(int j = 0;j<nVertices;j++)
		scanner.readInt(fileBuffer, &verticesIndices[j] );
	vis::Polyhedron* polyhedron = new vis::Polyhedron(pol->getPolyhedrons().size());
	std::vector<vis::Polygon*>& currentPolyhedronPolygons = polyhedron->getPolyhedronPolygons();
	for(std::vector<std::vector<int> >::size_type i = 0;i<verticesIndicesPerFace.size();i++){
		std::vector<int> aux;
		for(std::vector<int>::size_type j = 0;j<verticesIndicesPerFace[i].size();j++)
			aux.push_back(verticesIndices[verticesIndicesPerFace[i][j]]);
		vis::Polygon* polygon = hashingTree.getRegistry(aux);
		if(!polygon){
			polygon = ModelLoadingM3D::createPolygon(aux,vertices,
													 pol->getPolygons().size(),hashingTree);
			pol->getPolygons().push_back(polygon);
		}
		currentPolyhedronPolygons.push_back(polygon);
	}
	pol->getPolyhedrons().push_back(polyhedron);
}

void ModelLoadingM3D::readTetrahedron(PolyhedronMesh* pol,vis::ut::HashingTree<int,vis::Polygon>& hashingTree){
	std::vector<std::vector<int> > facesIndices = {{0,1,2},
												   {1,3,2},
												   {1,0,3},
												   {0,2,3}};
	readM3DPolyhedron(4,facesIndices,pol,hashingTree);

}
void ModelLoadingM3D::readPyramid(PolyhedronMesh* pol,vis::ut::HashingTree<int,vis::Polygon>& hashingTree){
	std::vector<std::vector<int> > facesIndices = {{0,1,2,3},
												   {1,4,2},
												   {2,4,3},
												   {3,4,0},
												   {0,4,1}};
	readM3DPolyhedron(5,facesIndices,pol,hashingTree);//*/
}

void ModelLoadingM3D::readPrism(PolyhedronMesh* pol,vis::ut::HashingTree<int,vis::Polygon>& hashingTree){
	std::vector<std::vector<int> > facesIndices = {{0,3,4,1},
												   {1,4,5,2},
												   {0,2,5,3},
												   {0,1,2},
												   {3,5,4}};
	readM3DPolyhedron(6,facesIndices,pol,hashingTree);
}
void ModelLoadingM3D::readHexahedron(PolyhedronMesh* pol,vis::ut::HashingTree<int,vis::Polygon>& hashingTree){
	std::vector<std::vector<int> > facesIndices = {{0,1,2,3},
												   {3,2,6,7},
												   {4,7,6,5},
												   {0,4,5,1},
												   {1,5,6,2},
												   {0,3,7,4}};
	readM3DPolyhedron(8,facesIndices,pol,hashingTree);
}

bool ModelLoadingM3D::readPolyhedrons( PolyhedronMesh* pol ) {
	if(!moveToElements(pol))
		return false;
	char word[250];
	std::vector<vis::Polygon*> &polygons = pol->getPolygons();
	std::vector<vis::Polyhedron*> &polyhedrons = pol->getPolyhedrons();
	polyhedrons.reserve(pol->getPolyhedronsCount());
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYHEDRON_MESH,
									 pol->getVerticesCount(),
									 pol->getPolyhedronsCount(),
									 pol->getPolyhedronsCount());
	emit setLoadedVertices(pol->getVerticesCount());
	vis::ut::HashingTree<int,vis::Polygon> hashingTree;
	for( int i = 0; i < pol->getPolyhedronsCount(); i++ ) {
		scanner.readString(fileBuffer,word,true);
		switch(word[0]){
			case 'T':{
				readTetrahedron(pol,hashingTree);
				break;
			}
			case 'P':{
				readPyramid(pol,hashingTree);
				break;
			}
			case 'R':{
				readPrism(pol,hashingTree);
				break;
			}
			case 'H':{
				readHexahedron(pol,hashingTree);
				break;
			}
			default:{
				break;
			}

		}
		scanner.skipToNextLine(fileBuffer);
		if(i%5000==0){
			emit setLoadedPolygons(i);
			emit setLoadedPolyhedrons(i);
		}
	}
	pol->setPolygonsCount(polygons.size());
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYHEDRON_MESH,
									 pol->getVerticesCount(),
									 pol->getPolygonsCount(),
									 pol->getPolyhedronsCount());
	emit setLoadedVertices(pol->getVerticesCount());
	emit setLoadedPolygons(pol->getPolygonsCount());
	emit setLoadedPolyhedrons(pol->getPolyhedronsCount());
	std::cout << "PolygonVector: Capacity = " << polygons.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << polygons.size() << std::endl;
	return true;
}
bool ModelLoadingM3D::readVertices( PolyhedronMesh* pol ) {
	int nvertices = 0;
	char word [256];
	scanner.readString(fileBuffer, word,true);
	scanner.skipToNextLine(fileBuffer);
	scanner.readInt(fileBuffer,&nvertices,true );
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYHEDRON_MESH,nvertices,0,0);
	pol->setVerticesCount( nvertices );
	std::vector<vis::Vertex*> &v = pol->getVertices();
	std::vector<float> &bounds = pol->getBounds();
	v.clear();
	v.reserve( nvertices );
	bounds.resize( 6 );
	int discard;
	scanner.readInt(fileBuffer, &discard, true );
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
		scanner.readInt(fileBuffer, &discard, true );
		scanner.readFloat(fileBuffer, &x );
		scanner.readFloat(fileBuffer, &y );
		scanner.readFloat(fileBuffer, &z );
		if(discard == 2){
			float x2,y2,z2;
			scanner.readFloat(fileBuffer, &x2 );
			scanner.readFloat(fileBuffer, &y2 );
			scanner.readFloat(fileBuffer, &z2 );
			x+=x2;y+=y2;z+=z2;
		}
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
		if(i%5000==0)
			emit setLoadedVertices(i);
	}
	emit setLoadedVertices(nvertices);
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingM3D] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
	return true;
}
bool ModelLoadingM3D::readBody( PolyhedronMesh* pol )
{
	if( readVertices( pol ) && readPolyhedrons( pol ) )
		return true;
	return false;
}
bool ModelLoadingM3D::moveToElements(PolyhedronMesh* pol){
	char word [256];
	while(scanner.readString(fileBuffer, word,true)){
		if( !strcmp( word, "[Elements,\0" ) ){
			scanner.readString(fileBuffer, word,true);
			if(!strcmp(word,"ARRAY1<STRING>]\0")){
				int nElements;
				scanner.readInt(fileBuffer,&nElements,true);
				pol->setPolyhedronsCount(nElements);
				return true;
			}
			return false;
		}
	}
	return false;
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingM3D);

