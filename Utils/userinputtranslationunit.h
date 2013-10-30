#ifndef USERINPUTTRANSLATIONUNIT_H
#define USERINPUTTRANSLATIONUNIT_H
#include <vector>
#include <string>
#include "Utils/keyboardstate.h"
#include <Qt>
class UserInputTranslationUnit
{
	public:
		UserInputTranslationUnit();
		UserInputTranslationUnit(int code, Qt::Key, bool s, bool c,
								 std::string cat,std::string desc);
		UserInputTranslationUnit(int code, int mousewheel, bool s, bool c,
								 std::string cat,std::string desc);
		UserInputTranslationUnit(int code, Qt::MouseButton, bool s, bool c,
								 std::string cat,std::string desc);
		~UserInputTranslationUnit();
		bool isItComplete(KeyboardState&);
		int getCode();
		std::string getDescription();
		std::string getCategory();
		Qt::Key getKey();
		bool isKeyValid();
		bool isMouseValid();
		Qt::MouseButton getMouseButton();
		bool isControl();
		bool isShift();
		int getWheel();
		void setup(Qt::Key, bool s, bool c);
		void setupWheel(int);
		void setup(Qt::MouseButton, int wheel, bool s, bool c);
		void setup(Qt::Key k,Qt::MouseButton m,int wheel, bool s, bool c);
		static const int UP_MOUSE_WHEEL = 1;
		static const int NO_MOUSE_WHEEL = 0;
		static const int DOWN_MOUSE_WHEEL = -1;
	private:
		bool shift;
		bool control;
		Qt::Key key;
		bool keyIsValid;
		Qt::MouseButton mouse;
		bool mouseIsValid;
		std::string category;
		std::string description;
		int code;
		int mouseWheel;
};

#endif // USERINPUTTRANSLATIONUNIT_H
