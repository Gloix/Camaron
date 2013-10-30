#ifndef LWVERTEX_H
#define LWVERTEX_H
#include "lwelement.h"
#include <glm/glm.hpp>
namespace vis{
class LWVertex: public LWElement {
	public:
		LWVertex( int id = -1, float x = 0.0f,
				float y = 0.0f, float z = 0.0f);
		virtual ~LWVertex();
		glm::vec3& getCoords();
		glm::vec3& getNormal();
		bool isUsed();
		void used();
		void setNormal(glm::vec3);
		bool isAtSurface();
	protected:
	private:
		bool usedb;
		glm::vec3 coords;
		glm::vec3 normal;
};
}
#endif // LWVERTEX_H
