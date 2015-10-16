#ifndef VERTEXCLOUD_H
#define VERTEXCLOUD_H
#include "Model/Model.h"
namespace vis{
class Vertex;
}

struct VScalarDef {
    int index;
    char name [256];
    std::vector<float> bounds;
};
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
        std::vector<VScalarDef*> &getScalarDefs();
        void addScalarDef(VScalarDef*);

		DOUBLE_DISPATCH_MODEL_DEC
	protected:
		bool _2d;
		std::vector<vis::Vertex*> vertices;
		int verticesCount;
        std::vector<VScalarDef*> scalarDefs;
	private:
};

#endif // VERTEXCLOUD_H
