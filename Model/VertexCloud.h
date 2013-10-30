#ifndef VERTEXCLOUD_H
#define VERTEXCLOUD_H
#include "Model/Model.h"
namespace vis{
class Vertex;
}
class VertexCloud: public Model
{
	public:
		VertexCloud(std::string);
		virtual ~VertexCloud();
		std::vector<vis::Vertex*>& getVertices();
		int getVerticesCount();
		void setVerticesCount( int );
		bool is2D();
		void set2D(bool);

		DOUBLE_DISPATCH_MODEL_DEC
	protected:
		bool _2d;
		std::vector<vis::Vertex*> vertices;
		int verticesCount;
	private:
};

#endif // VERTEXCLOUD_H
