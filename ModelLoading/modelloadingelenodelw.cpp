#include "ModelLoading/modelloadingelenodelw.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "Model/lightweightpolygonmesh.h"
#include "Model/lightweightpolyhedronMesh.h"
#include "Model/LWElements/lwvertex.h"
#include "Model/LWElements/lwpolygon.h"
#include "Utils/hashingtree.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <stdio.h>
#include <exception>
ModelLoadingEleNodeLW::ModelLoadingEleNodeLW():ModelLoadingStrategy()
{
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","ele"));
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","node"));
}

ModelLoadingEleNodeLW::~ModelLoadingEleNodeLW()
{
}

bool ModelLoadingEleNodeLW::validate(std::string filename){
	std::string filenameExt = FileUtils::getFileNameExtension(filename);
	if(filenameExt.compare("ele") == 0 || filenameExt.compare("node") == 0){
		std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
		if(FileUtils::fileExists(filenameNoExt+".ele")&&FileUtils::fileExists(filenameNoExt+".node"))
			return true;
	}
	return false;

}
Model* ModelLoadingEleNodeLW::load(std::string filename){
	std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
	std::cout << "File name: "<< filename << std::endl;
	std::cout << "File name No Ext: "<< filenameNoExt << std::endl;
	fileBufferEle = FileUtils::getFileToBuffer(filenameNoExt+".ele",&fileSizeEle);
	fileBufferNode = FileUtils::getFileToBuffer(filenameNoExt+".node",&fileSizeNode);
	readHeaderEle();
	readHeaderNode();
	Model* model = 0;
	try{
		if(isPolygonMesh){
			LightWeightPolygonMesh* polygonMeshModel = new LightWeightPolygonMesh(filename,numberOfNodes,numberOfElements);
			model = polygonMeshModel;
			emit setupProgressBarForNewModel(vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH,
											 numberOfNodes,
											 numberOfElements,0);
			if(!readVertices(polygonMeshModel)||!readPolygons(polygonMeshModel)){
				delete polygonMeshModel;
				model = (Model*)0;
			} else {
				completeMesh(polygonMeshModel);
			}
		}
		else{
			LightWeightPolyhedronMesh* polyhedronMeshModel = new LightWeightPolyhedronMesh(filename);
			model = polyhedronMeshModel;
			emit setupProgressBarForNewModel(vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH,
											 numberOfNodes,
											 numberOfElements,numberOfElements);
			if(!readVertices(polyhedronMeshModel)||!readPolyhedrons(polyhedronMeshModel)){
				delete polyhedronMeshModel;
				model = (Model*)0;
			}
			else{
				completeMesh(polyhedronMeshModel);
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
void ModelLoadingEleNodeLW::readHeaderEle(){
	scanner.reset(fileSizeEle);
	scanner.readInt(fileBufferEle,&numberOfElements,true);
	scanner.readInt(fileBufferEle,&numberOfNodesPerElement);
	scanner.readInt(fileBufferEle,&numberOfAttributesPerElement);
	isPolygonMesh = numberOfNodesPerElement == 3 || numberOfNodesPerElement == 6;
}
void ModelLoadingEleNodeLW::readHeaderNode(){
	scanner.reset(fileSizeNode);
	scanner.readInt(fileBufferNode,&numberOfNodes,true);
	scanner.readInt(fileBufferNode,&dimensions);
	scanner.readInt(fileBufferNode,&numberOfAttributesPerNode);
	scanner.readInt(fileBufferNode,&numberOfBoundaryMarkers);
}
bool ModelLoadingEleNodeLW::readVertices( LightWeightPolygonMesh* mesh){
	scanner.reset(fileSizeNode);
	scanner.readInt(fileBufferNode,&numberOfNodes,true);
	scanner.skipToNextLine(fileBufferNode);
	mesh->set2D(dimensions==2);
	std::vector<vis::LWVertex*>& vertices = mesh->getVertices();
	//vertices.reserve(numberOfNodes);
	indexVsPosition.clear();
	std::vector<float> &bounds = mesh->getBounds();
	bounds.resize( 6 );
	int index;
	float x,y;
	float z = 0.0f;
	scanner.readInt(fileBufferNode,&index,true);
	scanner.readFloat(fileBufferNode, &x );
	scanner.readFloat(fileBufferNode, &y );
	if(dimensions>2)
		scanner.readFloat(fileBufferNode, &z );
	vis::LWVertex* vertex = new vis::LWVertex( index, x, y, z );

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

		vertex = new vis::LWVertex( index, x, y, z );

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
bool ModelLoadingEleNodeLW::readPolygons( LightWeightPolygonMesh* mesh){
	scanner.reset(fileSizeEle);
	scanner.readInt(fileBufferEle,&numberOfElements);
	scanner.skipToNextLine(fileBufferEle);
	std::vector<vis::LWPolygon*>& triangles = mesh->getPolygons();
	std::vector<vis::LWVertex*>& vertices = mesh->getVertices();
	//vertices.reserve(numberOfNodes);
	for(int i = 0; i<numberOfElements;i++){
		int index,v1,v2,v3;
		scanner.readInt(fileBufferEle,&index,true);
		scanner.readInt(fileBufferEle,&v1);
		scanner.readInt(fileBufferEle,&v2);
		scanner.readInt(fileBufferEle,&v3);
		vis::LWPolygon* triangle = new vis::LWPolygon(index);
		std::vector<vis::LWVertex*>& triangleVertices = triangle->getVertices();
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
bool ModelLoadingEleNodeLW::readPolyhedrons( LightWeightPolyhedronMesh* mesh){
	scanner.reset(fileSizeEle);
	scanner.readInt(fileBufferEle,&numberOfElements);
	scanner.skipToNextLine(fileBufferEle);
	int polygonIndex = 0;
	std::vector<vis::LWPolygon*>& triangles = mesh->getPolygons();
	std::vector<vis::LWPolyhedron*>& tetrahedrons = mesh->getPolyhedrons();
	std::vector<vis::LWVertex*>& vertices = mesh->getVertices();
	//vertices.reserve(numberOfNodes);
	vis::ut::HashingTree<int,vis::LWPolygon> hashingTree;
	int v[4];
	for(int i = 0; i<numberOfElements;i++){
		int index;
		vis::LWPolyhedron* current = new vis::LWPolyhedron(index);
		std::vector<vis::LWPolygon*>& currentPolyhedronTriangles = current->getPolyhedronPolygons();
		scanner.readInt(fileBufferEle,&index,true);
		scanner.readInt(fileBufferEle,&v[0]);
		scanner.readInt(fileBufferEle,&v[1]);
		scanner.readInt(fileBufferEle,&v[2]);
		scanner.readInt(fileBufferEle,&v[3]);
		for(int j = 0;j<4;j++){
			std::vector<int> triangleVertices = {v[j],v[(j+1)%4],v[(j+2)%4]};
			vis::LWPolygon* polygon = hashingTree.getRegistry(triangleVertices);
			//if(7085413 < polygonIndex)
			//	polygon = triangles[0];
			if(!polygon){
				polygon = new vis::LWPolygon(polygonIndex++);
				hashingTree.registerValue(triangleVertices,polygon);
				triangles.push_back(polygon);
				std::vector<vis::LWVertex*>& currentPolygonVertices = polygon->getVertices();
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
	emit setupProgressBarForNewModel(vis::CONSTANTS::LIGHT_WEIGHT_POLYHEDRON_MESH,
									 mesh->getVerticesCount(),
									 mesh->getPolygonsCount(),
									 mesh->getPolyhedronsCount());
	emit setLoadedVertices(mesh->getVerticesCount());
	emit setLoadedPolygons(mesh->getPolygonsCount());
	emit setLoadedPolyhedrons(mesh->getPolyhedronsCount());
	indexVsPosition.clear();
	return true;
}
#include "Factories/ModelLoadingFactory.h"
REGISTER_LIGHT_WEIGHT_MODEL_LOADING_STRATEGY(ModelLoadingEleNodeLW);
