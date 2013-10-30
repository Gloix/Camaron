#ifndef LIGHTWEIGHTVERTEXCLOUD_H
#define LIGHTWEIGHTVERTEXCLOUD_H
#include "Model/Model.h"
#include "Model/LWElements/lwvertex.h"
class LightWeightVertexCloud: public Model
{
	public:
		LightWeightVertexCloud(std::string);
		virtual ~LightWeightVertexCloud();
		std::vector<vis::LWVertex*>& getVertices();
		int getVerticesCount();
		void setVerticesCount( int );
		bool is2D();
		void set2D(bool);
		virtual void clean();
		DOUBLE_DISPATCH_MODEL_DEC
	protected:
		bool _2d;
		std::vector<vis::LWVertex*> vertices;
		int verticesCount;
	private:
};

#endif // LIGHTWEIGHTVERTEXCLOUD_H
