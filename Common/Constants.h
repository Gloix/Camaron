#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
#define GCC_VERSION (__GNUC__ * 10000 \
							   + __GNUC_MINOR__ * 100 \
							   + __GNUC_PATCHLEVEL__)
#if GCC_VERSION > 40500
	#define USE_CONST_EXPR_FOR_STATIC_FLOAT
#endif
#define DEBUG_MOD
#define CAMERA_POSITION_X 0.0f
#define CAMERA_POSITION_Y 0.0f
#define CAMERA_POSITION_Z -0.5f
#define PI 3.1415926f
#define SHADER_RESOURCES ":Shaders/"
#define IMAGES_RESOURCES ":Images/"

#include <iostream>
namespace vis{
class CONSTANTS{
	public:
		static const int VERTEX = 0;
		static const int POLYGON = 1;
		static const int POLYHEDRON = 2;
		static const int NO_MODEL = -1;
		static const int VERTEX_CLOUD = 0;
		static const int POLYGON_MESH = 1;
		static const int POLYHEDRON_MESH = 2;
		static const int LIGHT_WEIGHT_VERTEX_CLOUD = 3;
		static const int LIGHT_WEIGHT_POLYGON_MESH = 4;
		static const int LIGHT_WEIGHT_POLYHEDRON_MESH = 5;
};
}
#endif // CONSTANTS_H_INCLUDED
