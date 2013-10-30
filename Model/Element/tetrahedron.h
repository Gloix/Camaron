#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "Model/Element/Polyhedron.h"
#include <vector>
namespace vis{
class Tetrahedron: public Polyhedron
{
	public:
		Tetrahedron(int nid);
		virtual ~Tetrahedron();
};
}
#endif // TETRAHEDRON_H
