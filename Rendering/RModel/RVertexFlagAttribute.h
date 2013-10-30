#ifndef RVERTEXFLAGATTRIBUTE_H
#define RVERTEXFLAGATTRIBUTE_H

#include <GL/glew.h>
class RVertexFlagAttribute{
	public:
		RVertexFlagAttribute(){ flagAttribute = 0u;}
		GLuint flagAttribute;
		void enableFlag(GLuint flag){flagAttribute |= flag;}
		void disableFlag(GLuint flag){flagAttribute &= ~flag;}
		bool isFlagEnabled(GLuint flag){return (flagAttribute&flag)==flag;}
		void disableAll(){flagAttribute = 0u;}
		//FLAGS:
		static const GLuint SELECTED_FLAG = 1u;
		static const GLuint VIRTUAL_OPOSITE_EDGE_FLAG = 2u;
		static const GLuint VIRTUAL_VERTEX = 4u;
		static const GLuint SURFACE_VERTEX = 8u;
		static const GLuint SURFACE_POLYGON = 16u;
		static const GLuint REPRESENTS_POLYGON = 32u;
		static const GLuint INSIDE_CONVEX_GEOMETRY = 64u;
};

#endif // RVERTEXFLAGATTRIBUTE_H
