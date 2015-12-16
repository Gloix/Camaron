#ifndef PROPERTYFIELDLOADINGTQS_H
#define PROPERTYFIELDLOADINGTQS_H
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "Utils/chararrayscanner.h"
#include <unordered_map>
struct VScalarDef;
class PropertyFieldLoadingTqs: public PropertyFieldLoadingStrategy
{
	public:
		PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(PropertyFieldLoadingTqs)
	protected:
	private:
		char* fileBuffer;
		long fileSize;
		void readHeader();

		CharArrayScanner scanner;

		//Ele header
		int numberOfElements;
		int numberOfAttributesPerElement;
		int numberOfNodesPerElement;
		//Node header
		int numberOfNodes;
		int dimensions;
		int numberOfAttributesPerNode;
		int numberOfBoundaryMarkers;
		std::unordered_map<int,int> indexVsPosition;
		std::vector<VScalarDef*> vertexProperties;

};

#endif // PROPERTYFIELDLOADINGTQS_H
