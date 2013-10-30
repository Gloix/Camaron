#include "lwelement.h"

vis::LWElement::LWElement(int idn)
{
	id = idn;
}

int vis::LWElement::getId(){
	return id;
}

void vis::LWElement::setId(int idn){
	id = idn;
}
