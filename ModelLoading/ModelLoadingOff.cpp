#include "ModelLoading/ModelLoadingOff.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Model/Element/Vertex.h"
#include "Model/PolygonMesh.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>

ModelLoadingOff::ModelLoadingOff():ModelLoadingStrategy("OFF", "off")
{
	fileBuffer = (char*)0;
	fileSize = 0;
}

ModelLoadingOff::~ModelLoadingOff(){}


bool ModelLoadingOff::validate(std::string filename)
{

	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	if(fileSize==0)
		return false;
	scanner.reset(fileSize);
	char word [256];
	bool valid = false;
	while(scanner.readString(fileBuffer, word,true)){
		if( !strcmp( word, "OFF\0" ) ){
			valid = true;
			break;
		}
	}
	delete[] fileBuffer;
	return valid;
}
Model* ModelLoadingOff::load(std::string filename){
	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	scanner.reset(fileSize);

	PolygonMesh* loaded = new PolygonMesh(filename);
	VertexCloud* vcloud = 0;
	try{
		readHeader(loaded);
		if(loaded->getPolygonsCount()){
			if( readBody( loaded ) ){
				delete[] fileBuffer;
				fileSize = 0;
				completeMesh(loaded);
				return ( Model* )loaded;
			}
		}else{
			//Vertex Cloud!
			vcloud = new VertexCloud(filename);
			vcloud->setVerticesCount(loaded->getVerticesCount());
			delete loaded;
			loaded = 0;
			if(readVertices( vcloud ) ){
				delete[] fileBuffer;
				fileSize = 0;
				return ( Model* )vcloud;
			}
		}
	}catch(std::bad_alloc &ba){
		if(loaded)
			delete loaded;
		if(vcloud)
			delete vcloud;
		delete[] fileBuffer;
		fileSize = 0;
		throw std::bad_alloc();
	}

	delete loaded;
	delete[] fileBuffer;
	fileSize = 0;
	return ( Model* )NULL;
}
bool ModelLoadingOff::readHeader(PolygonMesh* polygonMesh)
{

	int np = 0;
	int nf = 0;
	//read number of points
	scanner.readInt(fileBuffer,&np,true );
	//read number of faces}
	scanner.readInt(fileBuffer,&nf );
	polygonMesh->setPolygonsCount( nf );
	polygonMesh->setVerticesCount( np );

	//scanner.readInt(fileBuffer,&nf );//discard number of edges
	scanner.skipToNextLine(fileBuffer);
	int type = vis::CONSTANTS::POLYGON_MESH;
	if(nf<=0)
		type = vis::CONSTANTS::VERTEX_CLOUD;
	emit setupProgressBarForNewModel(type,np,nf,0);
	return true;
}

bool ModelLoadingOff::readPolygons( PolygonMesh* pol ) {
	int npf, pi;
	std::vector<vis::Polygon*> &vecPol = pol->getPolygons();
	vecPol.reserve(pol->getPolygonsCount());
	std::vector<vis::Vertex*> &verticesMesh = pol->getVertices();

	for( int i = 0; i < pol->getPolygonsCount(); i++ ) {
		scanner.readInt(fileBuffer, &npf );
		vis::Polygon* p;
		if(npf == 3)
			p = new vis::Triangle( i );
		else
			p = new vis::Polygon( i );
		std::vector<vis::Vertex*> &vertices = p->getVertices();
		vertices.reserve( npf );
		for( int j = 0; j < npf; j++ ) {
			scanner.readInt(fileBuffer, &pi );
			vertices.push_back( verticesMesh[pi] );
		}
		vecPol.push_back( p );
		scanner.skipToNextLine(fileBuffer);
		if(i%1000==0)
			emit setLoadedPolygons(i);
	}
	emit setLoadedPolygons(vecPol.size());
	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	return true;
}
bool ModelLoadingOff::readVertices( VertexCloud* pol ) {
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
	emit setLoadedVertices(v.size());
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

	return true;
}
bool ModelLoadingOff::readBody( PolygonMesh* pol )
{
	if( readVertices( pol ) && readPolygons( pol ) )
		return true;
	return false;
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingOff);

