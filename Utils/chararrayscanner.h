#ifndef CHARARRAYSCANNER_H
#define CHARARRAYSCANNER_H

class CharArrayScanner
{
	public:
		CharArrayScanner();
		void reset(long arraySize);
		bool readFloat(const char* source, float* target, bool ignoreEndLine = false);
		bool readInt(const char* source, int* target, bool ignoreEndLine = false);
		bool readString(const char* source, char* target, bool ignoreEndLine = false);
		template <class TYPE> bool readBinary(const char* source, TYPE* target);
		bool skipToNextLine(const char* source);
		bool invalidState();
		bool endOfLine(const char* source);
		bool move(int n);
	private:
		long max;
		long pos;
		char commentChar;
		bool findNextWordCurrentLine(const char* source);
		bool findNextWord(const char* source);
		bool findNextNumber(const char* source,bool ignoreEndLine = false);
		bool findNextFloatNumber(const char* source,bool ignoreEndLine = false);
		bool skipCharacter(const char c);
		bool skipCharacterButEndline(const char c);
		bool skipToEndOfLine(const char* source);
		char buffer[250];
};
template<class TYPE> bool CharArrayScanner::readBinary(const char* source, TYPE* target){
	*target = *((TYPE*)(source+pos));
	pos += sizeof(TYPE);
	return !invalidState();
}

#endif // CHARARRAYSCANNER_H
