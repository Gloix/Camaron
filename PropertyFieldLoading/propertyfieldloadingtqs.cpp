#include "PropertyFieldLoading/propertyfieldloadingtqs.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Utils/hashingtree.h"
#include "Utils/fileutils.h"
#include "PropertyFieldLoading/tqspropertyfielddef.h"
#include <iostream>
#include <stdio.h>
#include <exception>
PropertyFieldLoadingTqs::PropertyFieldLoadingTqs():PropertyFieldLoadingStrategy("TQS", "tqs")
{
}

PropertyFieldLoadingTqs::~PropertyFieldLoadingTqs()
{
}

bool PropertyFieldLoadingTqs::validate(std::string filename){
	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	if(fileSize==0)
		return false;
	scanner.reset(fileSize);
	char word [256];
	bool valid = false;
	scanner.readString(fileBuffer, word,false);
	if( !strcmp( word, "TQS\0" ) ) {
		float version = 0.0;
		scanner.readFloat(fileBuffer, &version, false);
		if(version == 1.0) {
			valid = true;
		}
	}
	delete[] fileBuffer;
	return valid;
}

std::vector<PropertyFieldDef> PropertyFieldLoadingTqs::loadDefs(std::string filename){
	//read number of points
	bool in = true;
	char word [256];
	//bool readingEdgeProperties = false;
	int propertyIndex = 0;
	bool error = false;
	std::string propertyName;
	int coordinatesSystem;
	int dimensions = 2;

	//START READING HEADER
	//skip "qst <version>" first
	scanner.skipToNextLine(fileBuffer);

	scanner.readString(fileBuffer, word,false);
	if( !strcmp( word, "name:\0" ) ){
		scanner.readString(fileBuffer, word,false);
		propertyName = std::string(word);
		scanner.skipToNextLine(fileBuffer);
	} else {
		throw ExceptionMessage("Name not defined in line 2");
	}

	scanner.readString(fileBuffer, word, false);
	if( !strcmp( word, "coordinates_system:\0" ) ){
		scanner.readString(fileBuffer, word,false);
		if( !strcmp(word, "barycentric") ) {
			coordinatesSystem = COORDINATES_SYSTEM_BARYCENTRIC;
		} else {
			throw ExceptionMessage("Coordinates system not recognized in line 3");
		}
		scanner.skipToNextLine(fileBuffer);
	} else {
		throw ExceptionMessage("Coordinates system not defined");
	}

	scanner.readString(fileBuffer, word, false);
	if( !strstr( word, "dimensions:\0" ) ){
		scanner.readInt(fileBuffer, &ndimensions,false);
		scanner.skipToNextLine(fileBuffer);
	} else {
		throw ExceptionMessage("Number of dimensions not defined in line 4");
	}
	TQSPropertyFieldDef property(0, propertyName, coordinatesSystem, dimensions);

	while(in){
		scanner.readString(fileBuffer, word, false);
		if( !strstr( word, "%\0" ) ){
			continue;
		}
		if( !strstr( word, "number_of_points:\0", false) ) {
			int numberOfPoints = 0;
			scanner.readInt(fileBuffer, &numberOfPoints, false);
			std::vector<std::vector<float>> values(numberOfPoints, std::vector<float>(dimensions+1));
			for( int i=0;i<numberOfPoints;i++ ) {
				for( int j=0;j<dimensions;j++ ) {
					float value = 0.0;
					scanner.readFloat(fileBuffer, &value, false);
					values[i][j] = value;
				}
			}
			property.addPolygonCoordinates();
		}else if( !strcmp( word, "property\0" ) ){
			//property name
			if(readingVertexProperties){
				scanner.readString(fileBuffer, word,false); // Skip datatype (we'll assume float for now)
				VScalarDef* scalarDef = new VScalarDef;
				scanner.readString(fileBuffer, scalarDef->name, false);
				vertexProperties.push_back(scalarDef);
				if( strcmp( scalarDef->name, "x\0" ) &&
						strcmp( scalarDef->name, "y\0") &&
						strcmp( scalarDef->name, "z\0")){
					scalarDef->index = propertyIndex++;
					polygonMesh->addScalarDef(scalarDef);
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
	polygonMesh->setAdditionalEdgesCount( ne );
	//emit setupProgressBarForNewModel(vis::CONSTANTS::POLYGON_MESH,np,nf,0);
	//scanner.readInt(fileBuffer,&nf );//discard number of edges
	if(!isBinary)
		scanner.skipToNextLine(fileBuffer);
	else{
		unsigned char dummychar;
		scanner.readBinary(fileBuffer, &dummychar);
		while(dummychar!='\n')
			scanner.readBinary(fileBuffer, &dummychar);
	}
	std::vector<PropertyFieldDef> result;
	result.push_back(property);
	return ;
}


void PropertyFieldLoadingTqs::load(std::string filename, Model* model, PropertyFieldDef propertyFieldDef){
	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
	scanner.reset(fileSize);

	vertexProperties.clear();
	try{
		if(readHeader(loaded)){
			if(loaded->getElementsCount()){
				if( readBody( loaded ) ){
					//this->completeVertexPolygonRelations(loaded);
					//emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
					//completePolygonPolygonRelations(loaded,4);
					//emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
					//this->shrinkVertexPolygonRelations(loaded);
					//calculateNormalsPolygons(loaded,4);
					//calculateNormalsVertices(loaded,4);
					//emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
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
void PropertyFieldLoadingTqs::readHeaderEle(){
	scanner.reset(fileSizeEle);
	scanner.readInt(fileBufferEle,&numberOfElements,true);
	scanner.readInt(fileBufferEle,&numberOfNodesPerElement);
	scanner.readInt(fileBufferEle,&numberOfAttributesPerElement);
	isPolygonMesh = numberOfNodesPerElement == 3 || numberOfNodesPerElement == 6;
}
void PropertyFieldLoadingTqs::readHeaderNode(){
	scanner.reset(fileSizeNode);
	scanner.readInt(fileBufferNode,&numberOfNodes,true);
	scanner.readInt(fileBufferNode,&dimensions);
	scanner.readInt(fileBufferNode,&numberOfAttributesPerNode);
	scanner.readInt(fileBufferNode,&numberOfBoundaryMarkers);
	int propertyIndex = 0;
	for(int i=0;i< numberOfAttributesPerNode;i++) {
		VScalarDef* vScalarDef = new VScalarDef();
		vScalarDef->index = propertyIndex++;
		sprintf(vScalarDef->name,"Property #%d",i);
		vertexProperties.push_back(vScalarDef);
	}
}
bool PropertyFieldLoadingTqs::readVertices( PolygonMesh* mesh){
	scanner.reset(fileSizeNode);
	scanner.readInt(fileBufferNode,&numberOfNodes,true);
	scanner.skipToNextLine(fileBufferNode);
	mesh->set2D(dimensions==2);
	std::vector<vis::Vertex*>& vertices = mesh->getVertices();
	//vertices.reserve(numberOfNodes);
	indexVsPosition.clear();
	std::vector<float> &bounds = mesh->getBounds();
	bounds.resize( 6 );
	int index;
	float x,y;
	float z = 0.0f;
	float prop = 0.0f;
	scanner.readInt(fileBufferNode,&index,true);
	scanner.readFloat(fileBufferNode, &x );
	scanner.readFloat(fileBufferNode, &y );
	if(dimensions>2)
		scanner.readFloat(fileBufferNode, &z );
	std::vector<VScalarDef*> &scalarDefs = mesh->getScalarDefs();
	vis::Vertex* vertex = new vis::Vertex( index, x, y, z, 0 );
	for(int i=0 ; i<numberOfAttributesPerNode; i++) {
		scanner.readFloat(fileBufferNode, &prop);
		//VScalar vscalar;
		//vscalar.fvalue = prop;
		vertex->addScalarProperty(i, prop);
		scalarDefs[i]->bounds.resize(2);
		scalarDefs[i]->bounds[0] = prop;
		scalarDefs[i]->bounds[1] = prop;
	}

	vertices.push_back(vertex);

	indexVsPosition[index] = 0;
	bounds[0] = bounds[3] = x;
	bounds[1] = bounds[4] = y;
	bounds[2] = bounds[5] = z;

	scanner.skipToNextLine(fileBufferNode);
	for(int i = 1; i<numberOfNodes;i++){
		scanner.readInt(fileBufferNode,&index,true);
		scanner.readFloat(fileBufferNode,&x);
		scanner.readFloat(fileBufferNode,&y);
		if(dimensions>2)
			scanner.readFloat(fileBufferNode,&z);
		indexVsPosition[index] = i;

		vertex = new vis::Vertex(index,x,y,z,i);
		for(int i=0 ; i<numberOfAttributesPerNode; i++) {
			scanner.readFloat(fileBufferNode, &prop);
			vertex->addScalarProperty(i, prop);
			if(scalarDefs[i]->bounds[0] > prop) {
				scalarDefs[i]->bounds[0] = prop;
			} else if(scalarDefs[i]->bounds[1] < prop) {
				scalarDefs[i]->bounds[1] = prop;
			}
		}

		vertices.push_back(vertex);
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
		scanner.skipToNextLine(fileBufferNode);
		if(i%5000==0)
			emit setLoadedVertices(i);
	}
	mesh->setVerticesCount(numberOfNodes);
	emit setLoadedVertices(numberOfNodes);
	return true;
}
bool PropertyFieldLoadingTqs::readPolygons( PolygonMesh* mesh){
	scanner.reset(fileSizeEle);
	scanner.readInt(fileBufferEle,&numberOfElements);
	scanner.skipToNextLine(fileBufferEle);
	std::vector<vis::Polygon*>& triangles = mesh->getPolygons();
	std::vector<vis::Vertex*>& vertices = mesh->getVertices();
	//vertices.reserve(numberOfNodes);
	for(int i = 0; i<numberOfElements;i++){
		int index,v1,v2,v3;
		scanner.readInt(fileBufferEle,&index,true);
		scanner.readInt(fileBufferEle,&v1);
		scanner.readInt(fileBufferEle,&v2);
		scanner.readInt(fileBufferEle,&v3);
		vis::Triangle* triangle = new vis::Triangle(index);
		std::vector<vis::Vertex*>& triangleVertices = triangle->getVertices();
		triangleVertices.push_back(vertices[indexVsPosition[v1]]);
		triangleVertices.push_back(vertices[indexVsPosition[v2]]);
		triangleVertices.push_back(vertices[indexVsPosition[v3]]);
		triangles.push_back(triangle);
		scanner.skipToNextLine(fileBufferEle);
		if(i%5000==0)
			emit setLoadedPolygons(i);
	}
	emit setLoadedPolygons(numberOfElements);
	indexVsPosition.clear();
	return true;
}
bool PropertyFieldLoadingTqs::readPolyhedrons( PolyhedronMesh* mesh){
	scanner.reset(fileSizeEle);
	scanner.readInt(fileBufferEle,&numberOfElements);
	scanner.skipToNextLine(fileBufferEle);
	int polygonIndex = 0;
	std::vector<vis::Polygon*>& triangles = mesh->getPolygons();
	std::vector<vis::Polyhedron*>& tetrahedrons = mesh->getPolyhedrons();
	std::vector<vis::Vertex*>& vertices = mesh->getVertices();
	//vertices.reserve(numberOfNodes);
	vis::ut::HashingTree<int,vis::Polygon> hashingTree;
	int v[4];
	for(int i = 0; i<numberOfElements;i++){
		int index;
		vis::Polyhedron* current = new vis::Polyhedron(index);
		std::vector<vis::Polygon*>& currentPolyhedronTriangles = current->getPolyhedronPolygons();
		scanner.readInt(fileBufferEle,&index,true);
		scanner.readInt(fileBufferEle,&v[0]);
		scanner.readInt(fileBufferEle,&v[1]);
		scanner.readInt(fileBufferEle,&v[2]);
		scanner.readInt(fileBufferEle,&v[3]);
		for(int j = 0;j<4;j++){
			std::vector<int> triangleVertices = {v[j],v[(j+1)%4],v[(j+2)%4]};
			vis::Polygon* polygon = hashingTree.getRegistry(triangleVertices);
			//if(7085413 < polygonIndex)
			//	polygon = triangles[0];
			if(!polygon){
				polygon = new vis::Triangle(polygonIndex++);
				hashingTree.registerValue(triangleVertices,polygon);
				triangles.push_back(polygon);
				std::vector<vis::Vertex*>& currentPolygonVertices = polygon->getVertices();
				currentPolygonVertices.push_back(vertices[indexVsPosition[triangleVertices[0]]]);
				currentPolygonVertices.push_back(vertices[indexVsPosition[triangleVertices[1]]]);
				currentPolygonVertices.push_back(vertices[indexVsPosition[triangleVertices[2]]]);
			}
			currentPolyhedronTriangles.push_back(polygon);
		}
		tetrahedrons.push_back(current);
		scanner.skipToNextLine(fileBufferEle);
		if(i%5000==0){
			emit setLoadedPolygons(i);
			emit setLoadedPolyhedrons(i);
		}
	}
	mesh->setPolygonsCount(polygonIndex);
	mesh->setPolyhedronsCount(tetrahedrons.size());
	//emit setupProgressBarForNewModel(vis::CONSTANTS::POLYHEDRON_MESH,
	//								 mesh->getVerticesCount(),
	//								 mesh->getPolygonsCount(),
	//								 mesh->getPolyhedronsCount());
	//emit setLoadedVertices(mesh->getVerticesCount());
	//emit setLoadedPolygons(mesh->getPolygonsCount());
	//emit setLoadedPolyhedrons(mesh->getPolyhedronsCount());
	indexVsPosition.clear();
	return true;
}
#include "Factories/PropertyFieldLoadingFactory.h"
REGISTER_PROPERTY_FIELD_LOADING_STRATEGY(PropertyFieldLoadingTqs);
