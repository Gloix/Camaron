#ifndef PRINTER_H
#define PRINTER_H
#include <glm/glm.hpp>

namespace vis{
class Vertex;
class Polygon;
class Element;
}
class Selection;

class Printer
{
	public:
		static void print( vis::Element* );
		static void print( vis::Vertex* );
		static void print( vis::Polygon* );
		static void print( Selection* );
		static void print(glm::mat4& matrix);
		static void print(glm::vec4&);
		static void print(glm::vec3& v);
	protected:
	private:
		Printer();
		virtual ~Printer();
};

#endif // PRINTER_H
