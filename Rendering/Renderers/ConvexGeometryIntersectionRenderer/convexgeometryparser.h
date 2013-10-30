#ifndef CONVEXGEOMETRYPARSER_H
#define CONVEXGEOMETRYPARSER_H
#include <string>
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h"
class GenericConvexGeometry;
class ConvexGeometryDefinedByPlanes;
class SphericConvexGeometry;
class ConvexGeometryParser
{
	public:
		static GenericConvexGeometry* getConvexGeometry(std::string&,
												 ConvexGeometryIntersectionRendererConfig* c);
	private:
		ConvexGeometryParser();
		static ConvexGeometryDefinedByPlanes* addPlanesPolygons(std::string& content,
														 ConvexGeometryIntersectionRendererConfig*);
		static ConvexGeometryDefinedByPlanes* addPlanesNormals(std::string& content,
														 ConvexGeometryIntersectionRendererConfig*);
		static SphericConvexGeometry* parseSphere(std::string& content,
														 ConvexGeometryIntersectionRendererConfig*);
		};

#endif // CONVEXGEOMETRYPARSER_H
