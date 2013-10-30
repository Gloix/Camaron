#include "chararrayscanner.h"
#include <cstdio>
#include <cstdlib>

#include <iostream>
#define MIN_CHARACTER_VALUE 42
CharArrayScanner::CharArrayScanner()
{
	pos = 0;
	this->max = 0L;
	commentChar = '#';
}

void CharArrayScanner::reset(long arraySize){
	pos = 0;
	this->max = arraySize;
}
bool CharArrayScanner::readFloat(const char* source, float* target,bool ignoreEndLine){
	//if(invalidState())//safe
	//	return false;
	if(findNextFloatNumber(source,ignoreEndLine)){
		if(readString(source,buffer,ignoreEndLine)){
			*target = atof(buffer);
			return true;
		}
	}
	return false;
}
bool CharArrayScanner::readString(const char* source, char* target,bool ignoreEndLine){
	//if(invalidState())//safe
	//	return false;//safe
	if(ignoreEndLine){
		while(pos < max && skipCharacter(source[pos]))
			pos++;;
	}
	else{
		while(pos < max && skipCharacterButEndline(source[pos]))
			pos++;
	}
	int i = 0;
	while(pos < max && source[pos]>MIN_CHARACTER_VALUE){
		target[i++] = source[pos++];
	}
	target[i] = '\0';
	//if(!ignoreEndLine)
	return findNextWordCurrentLine(source);
	//else
	//	return findNextWord(source);
}
bool CharArrayScanner::readInt(const char* source, int* target,bool ignoreEndLine){
	if(findNextNumber(source,ignoreEndLine)){
		int n = 0;
		int posSign = pos-1;
		while(pos < max && source[pos]>=48 && source[pos]<=57){
			n *= 10;
			n += ((int)source[pos++])-48;
		}
		if(source[posSign]=='-')
			n = -n;
		*target = n;
		//if(!ignoreEndLine)
		return findNextWordCurrentLine(source);
		//else
		//	return findNextWord(source);
	}
	return false;
}
bool CharArrayScanner::findNextWord(const char* source){
	while(pos < max && !skipCharacter(source[pos])){ //skip current word
		pos++;
		if(source[pos] == commentChar)
			skipToNextLine(source);
	}
	while(pos < max && skipCharacter(source[pos])){ //skip blank spaces to find next word
		pos++;
		if(source[pos] == commentChar)
			skipToNextLine(source);
	}
	return !invalidState();
}
bool CharArrayScanner::findNextWordCurrentLine(const char* source){
	while(pos < max && !skipCharacter(source[pos])){ //skip current word
		pos++;
		//std::cout << "Val Char: "<< source[pos] <<std::endl;
		if(source[pos] == commentChar)
			return skipToEndOfLine(source);
	}
	while(pos < max && skipCharacterButEndline(source[pos])){ //skip blank spaces to find next word
		pos++;
		//std::cout << "Val Char: "<< source[pos] <<std::endl;
		if(source[pos] == commentChar)
			return skipToEndOfLine(source);
	}
	return !invalidState();
}
bool CharArrayScanner::skipToEndOfLine(const char* source){
	while(pos < max && source[pos] != '\n' && source[pos]!='\r') //skip current line
		pos++;
	return !invalidState();
}

bool CharArrayScanner::skipCharacter(const char c){
	return c < MIN_CHARACTER_VALUE;
}
bool CharArrayScanner::skipCharacterButEndline(const char c){
	return c < MIN_CHARACTER_VALUE && c != '\n' && c!='\r';
}

bool CharArrayScanner::findNextNumber(const char* source,bool ignoreEndLine){
	//	while(pos < max && (((int)source[pos])<48 || ((int)source[pos])>57))//safe
	if(ignoreEndLine)
		while(source[pos]<45 || source[pos]>57)
			pos++;
	else{
		while((source[pos]<45 || source[pos]>57) && source[pos] != '\n' && source[pos] != '\r' )
			pos++;

		if(source[pos] == '\n' || source[pos] == '\r')
			return false;
	}
	return !invalidState();
}
bool CharArrayScanner::findNextFloatNumber(const char* source,bool ignoreEndLine){
	//	while(pos < max && (((int)source[pos])<45 || ((int)source[pos])>57))//safe
	if(ignoreEndLine)
		while(source[pos]<45 || source[pos]>57)
			pos++;
	else{
		while((source[pos]<45 || source[pos]>57) && source[pos] != '\n' && source[pos] != '\r' )
			pos++;
		if(source[pos] == '\n' || source[pos] == '\r')
			return false;
	}
	return !invalidState();
}

bool CharArrayScanner::invalidState(){
	return pos>=max;
}
bool CharArrayScanner::endOfLine(const char* source){
	return !invalidState() && (source[pos] == '\n' || source[pos] == '\r');
}
bool CharArrayScanner::skipToNextLine(const char* source){
	while(pos < max && source[pos] != '\n' && source[pos] != '\r'){
		pos++;
		//std::cout << "Val Char: "<< source[pos] <<std::endl;
	}
	while(pos < max && skipCharacter(source[pos])){ //skip blank spaces to find next word
		pos++;
		//std::cout << "Val Char: "<< source[pos] <<std::endl;
	}
	//std::cout << "At next line" <<std::endl;
	return !invalidState();
}
bool CharArrayScanner::move(int n){
	pos+=n;
	return !invalidState();
}

