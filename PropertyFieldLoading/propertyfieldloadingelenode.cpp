#include "PropertyFieldLoading/propertyfieldloadingelenode.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Utils/hashingtree.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <stdio.h>
#include <exception>
PropertyFieldLoadingEleNode::PropertyFieldLoadingEleNode():PropertyFieldLoadingStrategy()
{
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","ele"));
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","node"));
}

PropertyFieldLoadingEleNode::~PropertyFieldLoadingEleNode()
{
}

bool PropertyFieldLoadingEleNode::validate(std::string filename){
	std::string filenameExt = FileUtils::getFileNameExtension(filename);
	if(filenameExt.compare("ele") == 0 || filenameExt.compare("node") == 0){
		std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
		if(FileUtils::fileExists(filenameNoExt+".ele")&&FileUtils::fileExists(filenameNoExt+".node"))
			return true;
	}
	return false;

}
Model* PropertyFieldLoadingEleNode::load(std::string filename, Model* model){
	std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
	std::cout << "File name: "<< filename << std::endl;
	std::cout << "File name No Ext: "<< filenameNoExt << std::endl;
	fileBufferEle = FileUtils::getFileToBuffer(filenameNoExt+".ele",&fileSizeEle);
	fileBufferNode = FileUtils::getFileToBuffer(filenameNoExt+".node",&fileSizeNode);
	readHeaderEle();
	readHeaderNode();
	try{
		if(isPolygonMesh){
			PolygonMesh* polygonMeshModel = new PolygonMesh(filename,numberOfNodes,numberOfElements);
			for(std::vector<VScalarDef*>::size_type i=0; i<vertexProperties.size(); i++) {
				polygonMeshModel->addScalarDef(vertexProperties.at(i));
			}
			model = polygonMeshModel;
			//emit setupProgressBarForNewModel(vis::CONSTANTS::POLYGON_MESH,
			//								 numberOfNodes,
			//								 numberOfElements,0);
			if(!readVertices(polygonMeshModel)||!readPolygons(polygonMeshModel)){
				delete polygonMeshModel;
				model = (Model*)0;
			}
			else{
				//completeVertexPolygonRelations(polygonMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
				//completePolygonPolygonRelations(polygonMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
				//calculateNormalsPolygons(polygonMeshModel);
				//calculateNormalsVertices(polygonMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
			}
		}
		else{
			PolyhedronMesh* polyhedronMeshModel = new PolyhedronMesh(filename);
			for(std::vector<VScalarDef*>::size_type i=0; i<vertexProperties.size(); i++) {
				polyhedronMeshModel->addScalarDef(vertexProperties.at(i));
			}
			model = polyhedronMeshModel;
			//emit setupProgressBarForNewModel(vis::CONSTANTS::POLYHEDRON_MESH,
			//								 numberOfNodes,
			//								 numberOfElements,numberOfElements);
			if(!readVertices(polyhedronMeshModel)||!readPolyhedrons(polyhedronMeshModel)){
				delete polyhedronMeshModel;
				model = (Model*)0;
			}
			else{
				//completeVertexPolygonRelations(polyhedronMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
				//completePolygonPolygonRelations(polyhedronMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
				//completePolygonPolyhedronRelations(polyhedronMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYHEDRON_R);
				//calculateGeoCenterPolyhedronMesh(polyhedronMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::POLYHEDRON_GEOCENTER_CALCULATED);
				//calculateNormalsPolygons(polyhedronMeshModel);
				//fixSurfacePolygonsVerticesOrder(polyhedronMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::FIXED_SURFACE_POLYGONS_VERTICES_ORDER);
				//calculateNormalsVertices(polyhedronMeshModel);
				//emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
			}
		}
	}catch(std::bad_alloc& e){
		if(model)
			delete model;
		delete[] fileBufferEle;
		delete[] fileBufferNode;
		model = 0;
		throw e;
	}
	delete[] fileBufferEle;
	delete[] fileBufferNode;
	return model;
}
void PropertyFieldLoadingEleNode::readHeaderEle(){
	scanner.reset(fileSizeEle);
	scanner.readInt(fileBufferEle,&numberOfElements,true);
	scanner.readInt(fileBufferEle,&numberOfNodesPerElement);
	scanner.readInt(fileBufferEle,&numberOfAttributesPerElement);
	isPolygonMesh = numberOfNodesPerElement == 3 || numberOfNodesPerElement == 6;
}
void PropertyFieldLoadingEleNode::readHeaderNode(){
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
bool PropertyFieldLoadingEleNode::readVertices( PolygonMesh* mesh){
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
bool PropertyFieldLoadingEleNode::readPolygons( PolygonMesh* mesh){
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
bool PropertyFieldLoadingEleNode::readPolyhedrons( PolyhedronMesh* mesh){
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
REGISTER_PROPERTY_FIELD_LOADING_STRATEGY(PropertyFieldLoadingEleNode);
