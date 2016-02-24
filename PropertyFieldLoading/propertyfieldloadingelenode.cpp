#include "PropertyFieldLoading/propertyfieldloadingelenode.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "Model/PolygonMesh.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Utils/hashingtree.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <exception>
PropertyFieldLoadingEleNode::PropertyFieldLoadingEleNode():PropertyFieldLoadingStrategy()
{
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","node"));
}

PropertyFieldLoadingEleNode::~PropertyFieldLoadingEleNode()
{
}

bool PropertyFieldLoadingEleNode::validate(std::string filename){
	std::string filenameExt = FileUtils::getFileNameExtension(filename);
	if(filenameExt.compare("node") == 0){
		return true;
	}
	return false;

}

std::vector<std::shared_ptr<PropertyFieldDef>> PropertyFieldLoadingEleNode::loadDefs(std::string filename) {
	std::vector<std::shared_ptr<PropertyFieldDef>> vertexProperties;
	std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
	std::cout << "File name: "<< filename << std::endl;
	std::cout << "File name No Ext: "<< filenameNoExt << std::endl;
	std::ifstream fileNode(filenameNoExt+".node");
	readHeaderNode(fileNode, vertexProperties);
	return vertexProperties;
}

void PropertyFieldLoadingEleNode::load(std::string filename,
								   Model* model,
								   std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties){
	try{
		this->filename = filename;
		this->selectedProperties = selectedProperties;
		model->accept((ModelVisitor*)this);
	}catch(std::bad_alloc &ba){
		throw std::bad_alloc();
	}catch(ModelLoadingException &ex){
		throw ex;
	}
}

void PropertyFieldLoadingEleNode::readHeaderNode(std::ifstream& file, std::vector<std::shared_ptr<PropertyFieldDef>>& vertexProperties){
	//scanner.reset(fileSizeNode);
	std::string line;
	FileUtils::safeGetline(file, line);
	std::istringstream iss(line);
	iss >> numberOfNodes;
	iss >> dimensions;
	iss >> numberOfAttributesPerNode;
	iss >> numberOfBoundaryMarkers;
	// The next properties start with property index = dimensions+1 because
	// the vertex index, X, Y and/or Z should also be counted in as previous indices.
	int propertyIndex = dimensions+1;
	for(int i=0;i< numberOfAttributesPerNode;i++) {
		std::ostringstream propName;
		propName << "Property #" << i;
		std::shared_ptr<ScalarFieldDef> prop
				= std::make_shared<ScalarFieldDef> (propertyIndex++,
													propName.str(),
													numberOfNodes);
		std::shared_ptr<PropertyFieldDef> scalarProp
				= std::dynamic_pointer_cast<PropertyFieldDef>(prop);
		vertexProperties.push_back(scalarProp);
	}
}

void PropertyFieldLoadingEleNode::visit(VertexCloud* model) {
	readModelProperties(this->filename, model, this->selectedProperties);
}

void PropertyFieldLoadingEleNode::visit(PolygonMesh* model) {
	visit((VertexCloud*)model);
}

void PropertyFieldLoadingEleNode::visit(PolyhedronMesh* model) {
	visit((VertexCloud*) model);
}

bool PropertyFieldLoadingEleNode::readModelProperties( std::string filename, VertexCloud* vcloud, std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties) {
	std::vector<std::shared_ptr<ScalarFieldDef>> selectedScalars;
	for(std::shared_ptr<PropertyFieldDef> prop : selectedProperties) {
		selectedScalars.push_back(std::dynamic_pointer_cast<ScalarFieldDef>(prop));
	}

	std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
	std::cout << "File name: "<< filename << std::endl;
	std::cout << "File name No Ext: "<< filenameNoExt << std::endl;
	std::string filenameNode = filenameNoExt+".node";
	std::ifstream file(filenameNode);

	std::string line;
	// skip header
	FileUtils::safeGetline(file, line);
	auto totalProperties = selectedProperties.size();
	int totalProgress = 0;
	for(vis::Vertex* vertex : vcloud->getVertices()) {
		//propertyReader.setup(pol->getVertices()[i], startingIndex);
		// Skip X,Y and possibly Z. First real property has id 0
		int currPropId = 0;
		auto currInsertionPropId = vcloud->getPropertyFieldDefs().size();
		FileUtils::safeGetline(file, line);
		std::istringstream iss(line);
		for(std::shared_ptr<ScalarFieldDef> prop : selectedScalars) {
			float value;
			while(currPropId <= prop->getId()){
				iss >> value;
				currPropId++;
			}
			//std::vector<std::shared_ptr<PropertyFieldDef>>::iterator it = find(propertyIds.begin(), propertyIds.end(), i);
			vertex->addScalarProperty(currInsertionPropId++, value);
			prop->expandBounds(value);
		}
		totalProgress+=totalProperties;
		if(totalProgress%1000 == 0) {
			emit setLoadedProgress(totalProgress);
		}
	}
	for(std::shared_ptr<PropertyFieldDef> prop : selectedProperties) {
		vcloud->addPropertyFieldDef(prop);
	}
	return true;
}

#include "Factories/PropertyFieldLoadingFactory.h"
REGISTER_PROPERTY_FIELD_LOADING_STRATEGY(PropertyFieldLoadingEleNode);
