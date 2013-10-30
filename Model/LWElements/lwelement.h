#ifndef LWELEMENT_H
#define LWELEMENT_H

namespace vis{
class LWElement
{
	public:
		LWElement(int);
		int getId();
		void setId(int);
	protected:
		int id;
};
}
#endif // LWELEMENT_H
