#include "ModelLoading/modelloadingplylw.h"
#include "Model/LWElements/lwpolygon.h"
#include "Model/LWElements/lwvertex.h"
#include "Model/lightweightpolygonmesh.h"
#include "Utils/fileutils.h"
#include "Utils/endianess.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
ModelLoadingPlyLW::ModelLoadingPlyLW():ModelLoadingStrategy("PLY", "ply")
{
}
ModelLoadingPlyLW::~ModelLoadingPlyLW(){}


bool ModelLoadingPlyLW::validate(std::string filename)
{

	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	if(fileSize==0)
		return false;
	scanner.reset(fileSize);
	char word [256];
	bool valid = false;
	scanner.readString(fileBuffer, word,false);
	if( !strcmp( word, "ply\0" ) )
		valid = true;
	delete[] fileBuffer;
	return valid;
}
Model* ModelLoadingPlyLW::load(std::string filename){
	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	scanner.reset(fileSize);

	LightWeightPolygonMesh* loaded = new LightWeightPolygonMesh(filename);
	LightWeightVertexCloud* vcloud = 0;
	try{
		if(readHeader(loaded)){
			if(loaded->getPolygonsCount()){
				if( readBody( loaded ) ){
					delete[] fileBuffer;
					fileSize = 0;
					completeMesh(loaded);
					return ( Model* )loaded;
				}
			}else{
				//Vertex Cloud!
				vcloud = new LightWeightVertexCloud(filename);
				vcloud->setVerticesCount(loaded->getVerticesCount());
				delete loaded;
				loaded = 0;
				if(readVertices( vcloud ) ){
					delete[] fileBuffer;
					fileSize = 0;
					return ( Model* )vcloud;
				}
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
	}catch(ModelLoadingException &ex){
		delete loaded;
		delete[] fileBuffer;
		fileSize = 0;
		throw ex;
	}

	delete loaded;
	delete[] fileBuffer;
	fileSize = 0;
	return (Model*)NULL;
}
bool ModelLoadingPlyLW::readHeader(LightWeightPolygonMesh* polygonMesh)
{
	numberOfBytesInVertexPropertiesToIgnore = 0;
	int np = 0;
	int nf = 0;
	//read number of points
	bool in = true;
	char word [256];
	bool readingVertexProperties = false;
	while(in){
		scanner.readString(fileBuffer, word,false);
		if( !strcmp( word, "element\0" ) ){
			scanner.readString(fileBuffer, word,false);
			if( !strcmp( word, "vertex\0" ) ){
				scanner.readInt(fileBuffer,&np );
				readingVertexProperties = true;
			}
			else if( !strcmp( word, "face\0" ) ){
				scanner.readInt(fileBuffer,&nf );
				readingVertexProperties = false;
			}
		}else if( !strcmp( word, "property\0" ) ){
			//read type
			int propertySize = 0;
			scanner.readString(fileBuffer, word,false);
			if( !strcmp( word, "char\0" ) ||
					!strcmp( word, "uchar\0") ||
					!strcmp( word, "uint8\0")||
					!strcmp( word, "int8\0")){
				propertySize = sizeof(char);
			}
			else if( !strcmp( word, "int\0" ) ||
					 !strcmp( word, "float\0") ||
					 !strcmp( word, "uint\0")||
					 !strcmp( word, "float32\0")||
					 !strcmp( word, "int32\0")){
				propertySize = sizeof(int);
			}
			//property name
			if(readingVertexProperties){
				scanner.readString(fileBuffer, word,false);
				if( strcmp( word, "x\0" ) &&
						strcmp( word, "y\0")&&
						strcmp( word, "z\0")){
					//not a coordinate
					numberOfBytesInVertexPropertiesToIgnore += propertySize;
				}
			}

		}else if( !strcmp( word, "end_header\0" ) ){
			break;
		}else if(!strcmp( word, "format\0" )){
			scanner.readString(fileBuffer, word,false);
			isBinary = strcmp( word, "ascii\0" ) != 0;
		}
		scanner.skipToNextLine(fileBuffer);
		if(scanner.invalidState())
			return false;

	}
	polygonMesh->setPolygonsCount( nf );
	polygonMesh->setVerticesCount( np );
	emit setupProgressBarForNewModel(vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH,np,nf,0);
	//scanner.readInt(fileBuffer,&nf );//discard number of edges
	if(!isBinary)
		scanner.skipToNextLine(fileBuffer);
	else{
		unsigned char dummychar;
		scanner.readBinary(fileBuffer, &dummychar);
		while(dummychar!='\n')
			scanner.readBinary(fileBuffer, &dummychar);
	}
	return true;
}

bool ModelLoadingPlyLW::readPolygons( LightWeightPolygonMesh* pol ) {
	int npf, pi;
	std::vector<vis::LWPolygon*> &vecPol = pol->getPolygons();
	vecPol.reserve(pol->getPolygonsCount());
	std::vector<vis::LWVertex*> &verticesMesh = pol->getVertices();

	for( int i = 0; i < pol->getPolygonsCount(); i++ ) {
		scanner.readInt(fileBuffer, &npf );
		vis::LWPolygon* p = new vis::LWPolygon( i );
		std::vector<vis::LWVertex*> &vertices = p->getVertices();
		vertices.reserve( npf );
		for( int j = 0; j < npf; j++ ) {
			scanner.readInt(fileBuffer, &pi );
			vertices.push_back( verticesMesh[pi] );
		}
		vecPol.push_back( p );
		scanner.skipToNextLine(fileBuffer);
		if(i%5000==0)
			emit setLoadedPolygons(i);
	}
	emit setLoadedPolygons(pol->getPolygonsCount());
	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	return true;
}
bool ModelLoadingPlyLW::readVertices( LightWeightVertexCloud* pol ) {
	std::vector<vis::LWVertex*> &v = pol->getVertices();
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
	v.push_back( new vis::LWVertex( 0, x, y, z ) );
	bounds[0] = bounds[3] = x;
	bounds[1] = bounds[4] = y;
	bounds[2] = bounds[5] = z;
	scanner.skipToNextLine(fileBuffer);

	for( int i = 1; i < pol->getVerticesCount(); i++ ) {
		scanner.readFloat(fileBuffer, &x );
		scanner.readFloat(fileBuffer, &y );
		scanner.readFloat(fileBuffer, &z );
		v.push_back( new vis::LWVertex( i, x, y, z ) );
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
	emit setLoadedVertices(pol->getVerticesCount());
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingPlyLW] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
	return true;
}
bool ModelLoadingPlyLW::readPolygonsBinary( LightWeightPolygonMesh* pol ) {
	unsigned char npf;
	unsigned int pi;
	std::vector<vis::LWPolygon*> &vecPol = pol->getPolygons();
	vecPol.reserve(pol->getPolygonsCount());
	std::vector<vis::LWVertex*> &verticesMesh = pol->getVertices();
	int errors = 0;
	for( int i = 0; i < pol->getPolygonsCount(); i++ ) {
		scanner.readBinary(fileBuffer, &npf );
		vis::LWPolygon* p = new vis::LWPolygon( i-errors );
		std::vector<vis::LWVertex*> &vertices = p->getVertices();
		for( unsigned char j = 0; j < npf; j++ ) {
			scanner.readBinary(fileBuffer, &pi );
			pi = Endianess::reverseBytes(pi);
			//if(pi<verticesMesh.size())
				vertices.push_back( verticesMesh[pi] );
		}
		if(vertices.size()<3){
			//std::cout << "Alerta: "<<i<<",: "<<npf<<std::endl;
			delete p;
			errors++;
		}
		else{
			vecPol.push_back( p );
			if(vecPol.size()%5000==0)
				emit setLoadedPolygons(vecPol.size());
		}
		//saltar props inutles
	}
	if(errors>0)
		emit warningLoadingModel(QString("Ignored degenerated polygons: ")+QString::number(errors));
	pol->setPolygonsCount(vecPol.size());
	emit setLoadedPolygons(vecPol.size());
	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	return true;
}
bool ModelLoadingPlyLW::readVerticesBinary( LightWeightVertexCloud* pol ) {
	std::vector<vis::LWVertex*> &v = pol->getVertices();
	std::vector<float> &bounds = pol->getBounds();
	v.clear();
	v.reserve( pol->getVerticesCount() );
	bounds.resize( 6 );
	float x = 0.0f;
	scanner.readBinary(fileBuffer, &x );
	float y = 0.0f;
	scanner.readBinary(fileBuffer, &y );
	float z =0.0f;
	scanner.readBinary(fileBuffer, &z );
	x = Endianess::reverseBytes(x);
	y = Endianess::reverseBytes(y);
	z = Endianess::reverseBytes(z);
	v.push_back( new vis::LWVertex( 0, x, y, z ) );
	bounds[0] = bounds[3] = x;
	bounds[1] = bounds[4] = y;
	bounds[2] = bounds[5] = z;
	scanner.move(numberOfBytesInVertexPropertiesToIgnore);
	for( int i = 1; i < pol->getVerticesCount(); i++ ) {
		scanner.readBinary(fileBuffer, &x );
		scanner.readBinary(fileBuffer, &y );
		scanner.readBinary(fileBuffer, &z );
		x = Endianess::reverseBytes(x);
		y = Endianess::reverseBytes(y);
		z = Endianess::reverseBytes(z);
		v.push_back( new vis::LWVertex( i, x, y, z ) );
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
		scanner.move(numberOfBytesInVertexPropertiesToIgnore);
		if(i%1000==0)
			emit setLoadedVertices(i);
	}
	emit setLoadedVertices(pol->getVerticesCount());
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingPlyLW] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
	return true;
}
bool ModelLoadingPlyLW::readBody( LightWeightPolygonMesh* pol )
{
	if(isBinary){
		if( readVerticesBinary( pol ) && readPolygonsBinary( pol ) )
			return true;
	}else{

		if( readVertices( pol ) && readPolygons( pol ) )
			return true;
	}

	return false;
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_LIGHT_WEIGHT_MODEL_LOADING_STRATEGY(ModelLoadingPlyLW);
