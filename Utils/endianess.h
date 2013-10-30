#ifndef ENDIANESS_H
#define ENDIANESS_H

class Endianess
{
	public:
		template <class TYPE> static TYPE reverseBytes(const TYPE in);
		static bool isBigEndian();
		static unsigned char findEndianness();
		static const unsigned char LITTLE_ENDIAN = 1;
		static const unsigned char BIG_ENDIAN = 0;
	private:
		Endianess();
};

template <class TYPE> TYPE Endianess::reverseBytes(const TYPE in){
	TYPE retVal;
	char *floatToConvert = ( char* ) & in;
	char *returnFloat = ( char* ) & retVal;
	// swap the bytes into a temporary buffer
	int n = sizeof(TYPE);
	for(int i = 0;i<n;i++)
		returnFloat[i] = floatToConvert[n-1-i];
	return retVal;
}

#endif // ENDIANESS_H
