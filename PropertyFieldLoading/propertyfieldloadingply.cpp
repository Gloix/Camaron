#include "PropertyFieldLoading/propertyfieldloadingply.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/triangle.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/edge.h"
#include "Model/PolygonMesh.h"
#include "Utils/fileutils.h"
#include "Utils/endianess.h"
#include "ModelLoading/modelloadingply.h"
#include "Exceptions/exceptionmessage.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <regex>
PropertyFieldLoadingPly::PropertyFieldLoadingPly():PropertyFieldLoadingStrategy("PLY", "ply")
{
}
PropertyFieldLoadingPly::~PropertyFieldLoadingPly(){}


bool PropertyFieldLoadingPly::validate(std::string filename)
{
	return ModelLoadingPly().validate(filename);
//	fileBuffer = FileUtils::getFileToBuffer(filename,&fileSize);
//	if(fileSize==0)
//		return false;
//	scanner.reset(fileSize);
//	char word [256];
//	bool valid = false;
//	scanner.readString(fileBuffer, word,false);
//	if( !strcmp( word, "ply\0" ) )
//		valid = true;
//	delete[] fileBuffer;
//	return valid;
}

std::vector<std::shared_ptr<PropertyFieldDef>> PropertyFieldLoadingPly::loadDefs(std::string filename)
{
	std::ifstream file(filename);
	if(!file.is_open()) {
		throw ExceptionMessage("Could not open file");
	}
	int np = 0;
	bool readingVertexProperties = false;
	//bool readingEdgeProperties = false;
	int propertyIndex = 0;
	std::vector<std::shared_ptr<PropertyFieldDef>> vertexProperties;
	std::string line;
	std::string word;
	while(FileUtils::safeGetline(file, line)) {
		//std::istringstream iss(line);
		std::smatch sm;
		if( regex_match(line, sm,
						std::regex(R"(^\s*element\s+(\S+)\s*$)")) ) {
			readingVertexProperties = false;
			if(!sm[0].compare("vertex")) {
				readingVertexProperties = true;
			}
		}
		else if ( regex_match (line, sm,
								std::regex(R"(^\s*property\s+(\S+)\s+(\S+)\s*$)")) ) {
			if(readingVertexProperties) {
				if(sm[1].compare("x") && sm[1].compare("y") && sm[1].compare("z")) { //property name
					std::string propertyName = sm[1];
					std::shared_ptr<ScalarFieldDef> newPtr(new ScalarFieldDef(propertyIndex, propertyName, np));
					vertexProperties.push_back(newPtr);
				}
				propertyIndex++;
			}
		} else if( !line.compare("end_header" ) ) {
			break;
		} else if ( regex_match (line, sm,
									std::regex(R"(^\s*format\s+(\S+)\s*$)")) ) {
			bool isBinary = std::string(sm[0]).find("ascii") != 0;
			if(isBinary) {
				throw ExceptionMessage("Can't read properties from binary format file");
			}
		}
	}
	return vertexProperties;
}

void PropertyFieldLoadingPly::load(std::string filename, Model* model,
								   std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties)
{
	//vertexProperties.clear();
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

void PropertyFieldLoadingPly::visit(VertexCloud* model) {
	readModelProperties(this->filename, model, this->selectedProperties);
}

void PropertyFieldLoadingPly::visit(PolygonMesh* model) {
	readModelProperties(this->filename, model, this->selectedProperties);
}

void PropertyFieldLoadingPly::visit(PolyhedronMesh* model) {
	readModelProperties(this->filename, (VertexCloud*)model, this->selectedProperties);
}

bool PropertyFieldLoadingPly::readModelProperties( std::string filename, VertexCloud* pol, std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties) {
	std::ifstream file(filename);
	std::string line;
	// skip header
	while ( line.compare("end_header" ) ) {
		FileUtils::safeGetline(file, line);
	}
	PropertyReader propertyReader(&file);
	int startingIndex = pol->getPropertyFieldDefs().size();
	for(int i=0;i<pol->getVerticesCount();i++) {
		propertyReader.setup(pol->getVertices()[i], startingIndex);
		for(std::shared_ptr<PropertyFieldDef> prop : selectedProperties) {
			prop->accept(propertyReader);
		}
		//std::vector<float> properties = readVertexProperties(file, i, selectedProperties);

		//for(std::vector<float>::iterator it=properties.begin(); it<properties.end();++it ) {
		//	int propertyIndex = pol->getPropertyFieldDefs().size()+(it-properties.begin());
		//	pol->getVertices()[i]->addScalarProperty(propertyIndex,*it);
		//}
		if(i%1000 == 0) {
			emit setLoadedProgress(i);
		}
	}
	for(std::shared_ptr<PropertyFieldDef> prop : selectedProperties) {
		pol->addPropertyFieldDef(prop);
	}
	return true;
}

#include "Factories/PropertyFieldLoadingFactory.h"
REGISTER_PROPERTY_FIELD_LOADING_STRATEGY(PropertyFieldLoadingPly);
