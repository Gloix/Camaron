#include <cstring>
#include "convexgeometryparser.h"
#include "Utils/chararrayscanner.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometrydefinedbyplanes.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/sphericconvexgeometry.h"
ConvexGeometryParser::ConvexGeometryParser()
{
}

GenericConvexGeometry* ConvexGeometryParser::getConvexGeometry(std::string& content,
															   ConvexGeometryIntersectionRendererConfig* config){
	CharArrayScanner scanner;
	scanner.reset(content.size());
	char word[250];
	scanner.readString(content.c_str(),word,true);
	if(!std::strcmp( word, "POLYGONS\0" ))
		return ConvexGeometryParser::addPlanesPolygons(content,config);
	else if(!std::strcmp( word, "NORMALS\0" ))
		return ConvexGeometryParser::addPlanesNormals(content,config);
	else if(!std::strcmp( word, "SPHERE\0" ))
		return ConvexGeometryParser::parseSphere(content,config);
	return 0;
}
ConvexGeometryDefinedByPlanes* ConvexGeometryParser::addPlanesPolygons(std::string& content,
																	   ConvexGeometryIntersectionRendererConfig* config){
	ConvexGeometryDefinedByPlanes* convexGeometry = new ConvexGeometryDefinedByPlanes(config);
	CharArrayScanner scanner;
	convexGeometry->setFromNormal(false);
	scanner.reset(content.size());
	int nVertices,npolygons;
	scanner.readInt(content.c_str(),&nVertices,true);
	if(nVertices<=2 || nVertices > 10){
		delete convexGeometry;
		return 0; //TODO: THROW EXCEPTION WITH ERROR MESSAGE
	}
	std::vector<glm::vec3> vertices;
	for(int i = 0;i<nVertices;i++){
		float x,y,z;
		scanner.readFloat(content.c_str(),&x,true);
		scanner.readFloat(content.c_str(),&y,true);
		scanner.readFloat(content.c_str(),&z,true);
		vertices.push_back(glm::vec3(x,y,z));
	}
	scanner.readInt(content.c_str(),&npolygons,true);
	if(npolygons<=0 || npolygons > 10){
		delete convexGeometry;
		return 0; //TODO: THROW EXCEPTION WITH ERROR MESSAGE
	}
	std::vector<glm::vec3>& triangleVertices = convexGeometry->getTriangleVertices();
	for(int i = 0;i<npolygons;i++){
		int nverpol;
		std::vector<glm::vec3> polygon;
		scanner.readInt(content.c_str(),&nverpol,true);
		glm::vec3 currentPolygonGeoCenter = glm::vec3(0.0f,0.0f,0.0f);
		for(int j = 0;j<nverpol;j++){
			int currentVer;
			scanner.readInt(content.c_str(),&currentVer,true);
			if(currentVer<0 || currentVer >= (int)vertices.size()){
				//ERROR, vertice, numero polygono fuera de rago
				delete convexGeometry;
				return 0;
			}
			polygon.push_back(vertices[currentVer]);
			currentPolygonGeoCenter+=vertices[currentVer];
		}
		if(nverpol<3)
			continue;//throw error invalid polygon
		for(std::vector<glm::vec3>::size_type j = 1;j<polygon.size()-1;j++){
			triangleVertices.push_back(polygon[0]);
			triangleVertices.push_back(polygon[j]);
			triangleVertices.push_back(polygon[j+1]);
		}
		currentPolygonGeoCenter/=(float)nverpol;
		glm::vec3 normal = glm::normalize(glm::cross(polygon[0]-polygon[1],
													 polygon[2]-polygon[1]));
		convexGeometry->addPlane(Plane(currentPolygonGeoCenter,normal));
	}
	convexGeometry->uploadMainVertexData();
	return convexGeometry;
}
ConvexGeometryDefinedByPlanes* ConvexGeometryParser::addPlanesNormals(std::string& content,
																	  ConvexGeometryIntersectionRendererConfig* config){
	ConvexGeometryDefinedByPlanes* convexGeometry = new ConvexGeometryDefinedByPlanes(config);
	CharArrayScanner scanner;
	convexGeometry->setFromNormal(true);
	scanner.reset(content.size());
	int nPlanes;
	scanner.readInt(content.c_str(),&nPlanes,true);
	if(nPlanes<=0){
		delete convexGeometry;
		return 0; //TODO: THROW EXCEPTION WITH ERROR MESSAGE
	}

	for(int i = 0;i<nPlanes;i++){
		float x,y,z;
		scanner.readFloat(content.c_str(),&x,true);
		scanner.readFloat(content.c_str(),&y,true);
		scanner.readFloat(content.c_str(),&z,true);
		glm::vec3 origin(x,y,z);
		scanner.readFloat(content.c_str(),&x,true);
		scanner.readFloat(content.c_str(),&y,true);
		scanner.readFloat(content.c_str(),&z,true);
		glm::vec3 normal(x,y,z);
		convexGeometry->addPlane(Plane(origin,normal));
	}
	convexGeometry->calculateCorners();
	convexGeometry->uploadMainVertexData();
	return convexGeometry;
}

SphericConvexGeometry* ConvexGeometryParser::parseSphere(std::string& content,
														 ConvexGeometryIntersectionRendererConfig* config){
	SphericConvexGeometry* convexGeometry = new SphericConvexGeometry(config);
	float radio; float x,y,z;
	CharArrayScanner scanner;
	scanner.reset(content.size());
	scanner.readFloat(content.c_str(),&radio,true);
	radio = std::abs(radio);
	scanner.readFloat(content.c_str(),&x,true);
	scanner.readFloat(content.c_str(),&y,true);
	scanner.readFloat(content.c_str(),&z,true);
	int nTriangles = 1000;
	if(!scanner.invalidState())
		scanner.readInt(content.c_str(),&nTriangles,true);
	nTriangles = std::max(nTriangles,100);
	nTriangles = std::min(nTriangles,100000);
	convexGeometry->setSphereAttributes(radio,glm::vec3(x,y,z));
	convexGeometry->createVertexData(nTriangles);
	return convexGeometry;
}
