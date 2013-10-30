#include "rvertex.h"

RVertex::RVertex():coords(0.0f,0.0f,0.0f,0.0f)
{
	id = -1;
	pos = -1;
}
RVertex::RVertex(int id, int pos, float x, float y, float z, float h):coords(x,y,z,h)
{
	this->id = id;
	this->pos = pos;
}
