#ifndef RVERTEX_H
#define RVERTEX_H
#include <glm/glm.hpp>
class RVertex
{
	public:
		RVertex();
		RVertex(int id, int pos, float x, float y, float z, float h = 1.0f);
		glm::vec4 coords;
		int id;
		int pos;
};

#endif // RVERTEX_H
