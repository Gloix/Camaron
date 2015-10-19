#include "ModelLoading/modelloadingply.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/edge.h"
#include "Model/PolygonMesh.h"
#include "Utils/fileutils.h"
#include "Utils/endianess.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
ModelLoadingPly::ModelLoadingPly():ModelLoadingStrategy("PLY", "ply")
{
}
ModelLoadingPly::~ModelLoadingPly(){}


bool ModelLoadingPly::validate(std::string filename)
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
Model* ModelLoadingPly::load(std::string filename){
	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	scanner.reset(fileSize);

	PolygonMesh* loaded = new PolygonMesh(filename);
	VertexCloud* vcloud = 0;
    vertexProperties.clear();
	try{
		if(readHeader(loaded)){
			if(loaded->getPolygonsCount()){
				if( readBody( loaded ) ){
					this->completeVertexPolygonRelations(loaded);
					emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
					completePolygonPolygonRelations(loaded,4);
					emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
					//this->shrinkVertexPolygonRelations(loaded);
					calculateNormalsPolygons(loaded,4);
					calculateNormalsVertices(loaded,4);
					emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
					delete[] fileBuffer;
					fileSize = 0;
					return ( Model* )loaded;
				}
			}else{
				//Vertex Cloud!
				vcloud = new VertexCloud(filename);
				vcloud->setVerticesCount(loaded->getVerticesCount());
                vcloud->setAdditionalEdgesCount(loaded->getAdditionalEdgesCount());
				delete loaded;
				loaded = 0;
                if(readVertices( vcloud ) && readAdditionalEdges(vcloud)){
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
    // Delete x, y and z properties. The rest will be held by the Model object.
    for (std::vector<VScalarDef*>::size_type i = 0; i < vertexProperties.size() ; i++ ) {
        if( !strcmp(vertexProperties.at(i)->name, "x\0") ||
                !strcmp(vertexProperties.at(i)->name, "y\0") ||
                !strcmp(vertexProperties.at(i)->name, "z\0")) {
            delete vertexProperties.at(i);
        }
    }

	delete loaded;
	delete[] fileBuffer;
	fileSize = 0;
	return (Model*)NULL;
}
bool ModelLoadingPly::readHeader(PolygonMesh* polygonMesh)
{
	numberOfBytesInVertexPropertiesToIgnore = 0;
	int np = 0;
	int nf = 0;
    int ne = 0;
	//read number of points
	bool in = true;
	char word [256];
	bool readingVertexProperties = false;
    //bool readingEdgeProperties = false;
    int propertyIndex = 0;
	while(in){
		scanner.readString(fileBuffer, word,false);
		if( !strcmp( word, "element\0" ) ){
			scanner.readString(fileBuffer, word,false);
			if( !strcmp( word, "vertex\0" ) ){
				scanner.readInt(fileBuffer,&np );
				readingVertexProperties = true;
                //readingEdgeProperties = false;
            } else if( !strcmp( word, "face\0" ) ){
				scanner.readInt(fileBuffer,&nf );
				readingVertexProperties = false;
                //readingEdgeProperties = false;
            } else if( !strcmp( word, "edge\0" ) ){
                scanner.readInt(fileBuffer,&ne );
                //readingEdgeProperties = true;
                readingVertexProperties = false;
            }

		}else if( !strcmp( word, "property\0" ) ){
			//property name
			if(readingVertexProperties){
                scanner.readString(fileBuffer, word,false); // Skip datatype (we'll assume float for now)
                VScalarDef* scalarDef = new VScalarDef;
                scalarDef->index = propertyIndex++;
                vertexProperties.push_back(scalarDef);
                //scanner.readString(fileBuffer, word,false);
                scanner.readString(fileBuffer, scalarDef->name, false);
                if( strcmp( scalarDef->name, "x\0" ) &&
                        strcmp( scalarDef->name, "y\0") &&
                        strcmp( scalarDef->name, "z\0")){
                    polygonMesh->addScalarDef(scalarDef);
                }
                //if( strcmp( word, "x\0" ) &&
                //		strcmp( word, "y\0")&&
                //		strcmp( word, "z\0")){
					//not a coordinate
                    //numberOfBytesInVertexPropertiesToIgnore += propertySize;
                //}
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
    polygonMesh->setAdditionalEdgesCount( ne );
	emit setupProgressBarForNewModel(vis::CONSTANTS::POLYGON_MESH,np,nf,0);
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

bool ModelLoadingPly::readPolygons( PolygonMesh* pol ) {
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
		if(i%5000==0)
			emit setLoadedPolygons(i);
	}
	emit setLoadedPolygons(pol->getPolygonsCount());
	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	return true;
}
bool ModelLoadingPly::readVertices( VertexCloud* pol ) {
	std::vector<vis::Vertex*> &v = pol->getVertices();
	std::vector<float> &bounds = pol->getBounds();
    std::vector<VScalarDef*> &scalarDefs = pol->getScalarDefs();
	v.clear();
	v.reserve( pol->getVerticesCount() );
	bounds.resize( 6 );

//	float x = 0.0f;
//    //scanner.readFloat(fileBuffer, &x );
//	float y = 0.0f;
//    //scanner.readFloat(fileBuffer, &y );
//    float z = 0.0f;
//    //scanner.readFloat(fileBuffer, &z );
//    for( uint i = 0; i < vertexProperties.size(); i++ ) {
//        if( !strcmp(vertexProperties.at(i).name, "x\0")) {
//            scanner.readFloat(fileBuffer, &x);
//        } else if( !strcmp(vertexProperties.at(i).name, "y\0")) {
//            scanner.readFloat(fileBuffer, &y);
//        } else if( !strcmp(vertexProperties.at(i).name, "z\0")) {
//            scanner.readFloat(fileBuffer, &z);
//        }
//    }
    float x, y, z;
    vis::Vertex *vertex = readVertex(0, x, y, z);
    for(std::vector<VScalarDef*>::size_type i=0 ; i<scalarDefs.size() ; i++) {
        scalarDefs[i]->bounds.resize(2);
        scalarDefs[i]->bounds[0] = scalarDefs[i]->bounds[1] = vertex->getProperty(i);
    }

    v.push_back( vertex );
    //v.push_back( new vis::Vertex( 0, x, y, z ) );
	bounds[0] = bounds[3] = x;
	bounds[1] = bounds[4] = y;
	bounds[2] = bounds[5] = z;
	scanner.skipToNextLine(fileBuffer);

	for( int i = 1; i < pol->getVerticesCount(); i++ ) {
//		scanner.readFloat(fileBuffer, &x );
//		scanner.readFloat(fileBuffer, &y );
//		scanner.readFloat(fileBuffer, &z );
//		v.push_back( new vis::Vertex( i, x, y, z ) );
        vertex = readVertex(i, x, y, z);
        for(std::vector<VScalarDef*>::size_type i=0 ; i<scalarDefs.size() ; i++) {
            if ( vertex->getProperty(i) > scalarDefs[i]->bounds[1] ) {
                scalarDefs[i]->bounds[1] = vertex->getProperty(i);
            } else if ( vertex->getProperty(i) < scalarDefs[i]->bounds[0] ) {
                scalarDefs[i]->bounds[0] = vertex->getProperty(i);
            }
        }
        v.push_back( vertex );

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
	std::cout << "[ModelLoadingPly] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
	return true;
}

vis::Vertex* ModelLoadingPly::readVertex(int id, float& x, float& y, float& z) {
    x = 0.0f;
    //scanner.readFloat(fileBuffer, &x );
    y = 0.0f;
    //scanner.readFloat(fileBuffer, &y );
    z = 0.0f;
    //scanner.readFloat(fileBuffer, &z );
    //std::vector<VScalar> properties;
    std::vector<float> properties;
    for( std::vector<VScalarDef*>::size_type i = 0; i < vertexProperties.size(); i++ ) {
        if( !strcmp(vertexProperties.at(i)->name, "x\0")) {
            scanner.readFloat(fileBuffer, &x);
        } else if( !strcmp(vertexProperties.at(i)->name, "y\0")) {
            scanner.readFloat(fileBuffer, &y);
        } else if( !strcmp(vertexProperties.at(i)->name, "z\0")) {
            scanner.readFloat(fileBuffer, &z);
        } else {
            //VScalar vprop;
            float f;
            scanner.readFloat(fileBuffer, &f);
            properties.push_back(f);
        }
    }
    vis::Vertex* vertex = new vis::Vertex(id, x, y, z);
    for( unsigned int i = 0; i < properties.size(); i++) {
        vertex->addProperty(i,properties[i]);
    }
    return vertex;
}

bool ModelLoadingPly::readAdditionalEdges( VertexCloud* pol ) {
    std::vector<vis::Edge*> &e = pol->getAdditionalEdges();
    e.clear();
    e.reserve(pol->getAdditionalEdgesCount());

    int n0, n1;
    int r, g, b;
    for( int i = 0; i < pol->getAdditionalEdgesCount(); i++ ) {
        scanner.readInt(fileBuffer, &n0);
        scanner.readInt(fileBuffer, &n1);
        scanner.readInt(fileBuffer, &r);
        scanner.readInt(fileBuffer, &g);
        scanner.readInt(fileBuffer, &b);
        vis::Edge * edge = new vis::Edge(i, pol->getVertices()[n0],
                       pol->getVertices()[n1],
                       glm::vec3(r/255.0f,g/255.0f,b/255.0f));
        e.push_back( edge );
        scanner.skipToNextLine(fileBuffer);
        //if(i%1000==0)
            //emit setLoadedAdditionalEdges(i);
    }

    //emit setLoadedAdditionalEdges(pol->getAdditionalEdgesCount());
    std::cout << "AdditionalEdgesVector: Capacity = " << e.capacity() << std::endl;
    std::cout << "AdditionalEdgesVector: Size = " << e.size() << std::endl;
    return true;
}

bool ModelLoadingPly::readPolygonsBinary( PolygonMesh* pol ) {
	unsigned char npf;
	unsigned int pi;
	std::vector<vis::Polygon*> &vecPol = pol->getPolygons();
	vecPol.reserve(pol->getPolygonsCount());
	std::vector<vis::Vertex*> &verticesMesh = pol->getVertices();
	int errors = 0;
	for( int i = 0; i < pol->getPolygonsCount(); i++ ) {
		scanner.readBinary(fileBuffer, &npf );
		vis::Polygon* p;
		if(npf == 3)
			p = new vis::Triangle( i-errors );
		else
			p = new vis::Polygon( i-errors );
		std::vector<vis::Vertex*> &vertices = p->getVertices();
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
bool ModelLoadingPly::readVerticesBinary( VertexCloud* pol ) {
	std::vector<vis::Vertex*> &v = pol->getVertices();
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
	v.push_back( new vis::Vertex( 0, x, y, z ) );
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
		scanner.move(numberOfBytesInVertexPropertiesToIgnore);
		if(i%1000==0)
			emit setLoadedVertices(i);
	}
	emit setLoadedVertices(pol->getVerticesCount());
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingPly] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
	return true;
}
bool ModelLoadingPly::readBody( PolygonMesh* pol )
{
	if(isBinary){
		if( readVerticesBinary( pol ) && readPolygonsBinary( pol ) )
			return true;
	}else{

        if( readVertices( pol ) && readPolygons( pol ) && readAdditionalEdges( pol ) )
			return true;
	}

	return false;
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingPly);
