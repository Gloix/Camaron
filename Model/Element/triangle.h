#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Common/Constants.h"
#include "Model/Element/Polygon.h"
namespace vis{
class Triangle: public Polygon
{
	public:
		Triangle(int newid);
		virtual ~Triangle();
		virtual float getArea();
		float getLMin();//smallest side
		float getLMax();//longest side
		float getLMid();//middle side

		virtual void invertVerticesOrder();

		DOUBLE_DISPATCH_ELEMENT_DEC
	private:
		void setSides();
#ifdef USE_CONST_EXPR_FOR_STATIC_FLOAT
		static constexpr float NULL_SIDE = -1.0f;
#else
		static const float NULL_SIDE = -1.0f;
#endif
		float lmin,lmax,lmid;
};
}
#endif // TRIANGLE_H
