#include "modelloadingtri.h"
#include "Utils/fileutils.h"
#include "Model/PolygonMesh.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/triangle.h"
#include <iostream>

ModelLoadingTri::ModelLoadingTri():
	ModelLoadingStrategy("Triangle Mesh","tri")
{
}
ModelLoadingTri::~ModelLoadingTri()
{
}
bool ModelLoadingTri::validate(std::string)
{
	return true;
}
Model* ModelLoadingTri::load(std::string filename){
	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	scanner.reset(fileSize);

	PolygonMesh* loaded = new PolygonMesh(filename);
	if( readBody( loaded ) ){
		this->completeVertexPolygonRelations(loaded);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
		completePolygonPolygonRelations(loaded);
		emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
		//this->shrinkVertexPolygonRelations(loaded);
		calculateNormalsPolygons(loaded);
		calculateNormalsVertices(loaded);
		emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
		delete[] fileBuffer;
		fileSize = 0;
		return ( Model* )loaded;
	}
	delete loaded;
	delete[] fileBuffer;
	fileSize = 0;
	return ( Model* )0;
}


bool ModelLoadingTri::readPolygons( PolygonMesh* pol ) {
	int npol;
	scanner.readInt(fileBuffer, &npol, true );
	scanner.skipToNextLine(fileBuffer);
	pol->setPolygonsCount(npol);
	std::vector<vis::Polygon*> &vecPol = pol->getPolygons();
	vecPol.reserve(pol->getPolygonsCount());
	std::vector<vis::Vertex*> &verticesMesh = pol->getVertices();
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYGON_MESH,pol->getVerticesCount(),npol,0);
	emit setLoadedVertices(pol->getVerticesCount());
	for( int i = 0; i < pol->getPolygonsCount(); i++ ) {
		int v1,v2,v3;
		scanner.readInt(fileBuffer, &v1,true );
		scanner.readInt(fileBuffer, &v2 );
		scanner.readInt(fileBuffer, &v3 );
		vis::Triangle* p = new vis::Triangle( i );
		std::vector<vis::Vertex*> &vertices = p->getVertices();
		vertices.push_back( verticesMesh[v1] );
		vertices.push_back( verticesMesh[v2] );
		vertices.push_back( verticesMesh[v3] );
		vecPol.push_back( p );
		scanner.skipToNextLine(fileBuffer);
		if(i%10000==0)
			emit setLoadedPolygons(i);
	}
	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	emit setLoadedPolygons(npol);
	return true;
}
bool ModelLoadingTri::readVertices( PolygonMesh* pol ) {
	int nver;
	scanner.readInt(fileBuffer, &nver, true );
	scanner.skipToNextLine(fileBuffer);
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYGON_MESH,nver,0,0);
	std::vector<vis::Vertex*> &v = pol->getVertices();
	std::vector<float> &bounds = pol->getBounds();
	v.clear();
	v.reserve( nver );
	pol->setVerticesCount(nver);
	bounds.resize( 6 );
	float x,y,z;
	scanner.readFloat(fileBuffer, &x, true );
	scanner.readFloat(fileBuffer, &y );
	scanner.readFloat(fileBuffer, &z );
	v.push_back( new vis::Vertex( 0, x, y, z ) );
	bounds[0] = bounds[3] = x;
	bounds[1] = bounds[4] = y;
	bounds[2] = bounds[5] = z;
	scanner.skipToNextLine(fileBuffer);

	for( int i = 1; i < pol->getVerticesCount(); i++ ) {
		scanner.readFloat(fileBuffer, &x,true );
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
	emit setLoadedVertices(nver);
	return true;
}
bool ModelLoadingTri::readBody( PolygonMesh* pol )
{
	if( readVertices( pol ) && readPolygons( pol ) )
		return true;
	return false;
}
#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingTri);
