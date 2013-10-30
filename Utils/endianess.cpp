#include "Utils/endianess.h"

const unsigned char Endianess::BIG_ENDIAN = 0;
const unsigned char Endianess::LITTLE_ENDIAN = 1;

Endianess::Endianess()
{
}
bool Endianess::isBigEndian(){
	union {
		int i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1;
}

unsigned char Endianess::findEndianness(){
	return (Endianess::isBigEndian())?Endianess::BIG_ENDIAN:Endianess::LITTLE_ENDIAN;
}
