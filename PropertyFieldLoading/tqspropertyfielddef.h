#ifndef TQSPROPERTYFIELDDEF_H
#define TQSPROPERTYFIELDDEF_H

#include<string>
#include<map>
#include "PropertyFieldLoading/propertyfielddef.h"
#include<vector>

#define COORDINATES_SYSTEM_BARYCENTRIC 0

class TQSPropertyFieldDef : public PropertyFieldDef, public std::enable_shared_from_this<TQSPropertyFieldDef>
{
	public:
		TQSPropertyFieldDef(int id, std::string name, int elements, int coordinatesSystem, int dimensions);
		void addPolygonCoordinates(int sides, std::vector<std::vector<float>> values);
		std::vector<std::vector<float>>* getCoordinatesForPolygon(int sides);
		DOUBLE_DISPATCH_PROPDEF_DEC

	private:
		std::map<int,std::vector<std::vector<float>>> valuesPerPolygonType;
		int coordinatesSystem;
		int dimensions;

};

#endif // TQSPROPERTYFIELDDEF_H
