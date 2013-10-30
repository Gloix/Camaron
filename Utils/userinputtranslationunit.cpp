#include "userinputtranslationunit.h"

UserInputTranslationUnit::UserInputTranslationUnit(){}
UserInputTranslationUnit::UserInputTranslationUnit(int code,
												   int mousewheel,
												   bool s, bool c,
												   std::string cat,
												   std::string desc){
	this->code = code;
	shift = s;
	control = c;
	category = cat;
	description = desc;
	mouseIsValid = true;
	mouse = Qt::NoButton;
	keyIsValid = false;
	mouseWheel = mousewheel;
}

UserInputTranslationUnit::UserInputTranslationUnit(int code,
												   Qt::Key k,
												   bool s,
												   bool c,
												   std::string cat,
												   std::string desc)
{
	this->code = code;
	key = k;
	shift = s;
	control = c;
	category = cat;
	description = desc;
	mouseIsValid = false;
	mouse = Qt::NoButton;
	keyIsValid = true;
	mouseWheel = 0;
}
UserInputTranslationUnit::UserInputTranslationUnit(int code,
												   Qt::MouseButton k,
												   bool s,
												   bool c,
												   std::string cat,
												   std::string desc)
{
	this->code = code;
	mouse = k;
	shift = s;
	control = c;
	category = cat;
	description = desc;
	mouseIsValid = true;
	keyIsValid = false;
	mouseWheel = 0;
}
UserInputTranslationUnit::~UserInputTranslationUnit(){

}

bool UserInputTranslationUnit::isItComplete(KeyboardState& kstate){
	if(keyIsValid&&!kstate.isKeyPressed(key))
		return false;
	if(mouseIsValid&&!kstate.isKeyPressed(mouse))
		return false;
	if(shift && !kstate.isKeyPressed(Qt::Key_Shift))
		return false;
	if(control && !kstate.isKeyPressed(Qt::Key_Control))
		return false;
	return true;
}

int UserInputTranslationUnit::getCode(){
	return code;
}

bool UserInputTranslationUnit::isControl(){
	return control;
}

bool UserInputTranslationUnit::isShift(){
	return shift;
}
Qt::Key UserInputTranslationUnit::getKey(){
	return key;
}

Qt::MouseButton UserInputTranslationUnit::getMouseButton(){
	return mouse;
}

bool UserInputTranslationUnit::isKeyValid(){
	return keyIsValid;
}

bool UserInputTranslationUnit::isMouseValid(){
	return mouseIsValid;
}

void UserInputTranslationUnit::setup(Qt::Key k, bool s, bool c){
	key = k;
	shift = s;
	control = c;
	mouseIsValid = false;
	keyIsValid = true;
}
void UserInputTranslationUnit::setup(Qt::MouseButton m,int wheel, bool s, bool c){
	mouse = m;
	shift = s;
	control = c;
	mouseIsValid = true;
	keyIsValid = false;
	mouseWheel = wheel;
}

void UserInputTranslationUnit::setup(Qt::Key k,Qt::MouseButton m,int wheel, bool s, bool c){
	mouse = m;
	key = k;
	shift = s;
	control = c;
	mouseIsValid = true;
	keyIsValid = true;
	mouseWheel = wheel;
}
void UserInputTranslationUnit::setupWheel(int s){
	mouseWheel = s;
}

int UserInputTranslationUnit::getWheel(){
	return mouseWheel;
}

std::string UserInputTranslationUnit::getDescription(){
	return description;

}

std::string UserInputTranslationUnit::getCategory(){
	return category;
}
