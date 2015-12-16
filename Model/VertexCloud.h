#ifndef VERTEXCLOUD_H
#define VERTEXCLOUD_H
#include "Model/Model.h"
#include <memory>
namespace vis{
class Vertex;
class Edge;
}

class VertexCloud: public Model
{
	public:
		VertexCloud(std::string);
		virtual ~VertexCloud();
		std::vector<vis::Vertex*>& getVertices();
		std::vector<vis::Edge*>& getAdditionalEdges();
		int getVerticesCount();
		int getAdditionalEdgesCount();
		void setVerticesCount( int );
		void setAdditionalEdgesCount( int );
		bool is2D();
		void set2D(bool);

		DOUBLE_DISPATCH_MODEL_DEC
	protected:
		bool _2d;
		std::vector<vis::Vertex*> vertices;
		int verticesCount;
		int additionalEdgesCount;
		std::vector<vis::Edge*> additionalEdges;
	private:
};

#endif // VERTEXCLOUD_H
