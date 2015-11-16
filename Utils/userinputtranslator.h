#ifndef USERINPUTTRANSLATOR_H
#define USERINPUTTRANSLATOR_H
#include <vector>
#include <string>
#include "Utils/userinputtranslationunit.h"
#include "Utils/keyboardstate.h"
#include <map>

#define ADD_USER_INPUT_DIC_UNIT(category,description,code,key, shift, control)	  \
{															 \
	UserInputTranslationUnit userinputunit(code,key,shift,control,\
	std::string(category),std::string(description));\
	if(keysDictionary.count(category))\
		keysDictionary[category].push_back(userinputunit);\
	else{\
		std::vector<UserInputTranslationUnit> newvector;\
		newvector.push_back(userinputunit);\
		keysDictionary.insert(std::make_pair(category,newvector));\
	}\
}
class UserInputTranslator//singleton
{
	public:
		static UserInputTranslator* getSingletonInstance();
		static void deleteSingletonInstance();
		int getTranslatedAction(KeyboardState&);
		bool getConfigFromFile(std::string);
		static bool saveConfigToFile(std::string);
		std::map<std::string,std::vector<UserInputTranslationUnit> >& getDictionary();
		static const int NULL_ACTION = -1;
		static const int CAMERA_TRA_Z_INC = 1;
		static const int CAMERA_TRA_Z_DEC = 2;
		static const int CAMERA_TRA_Y_INC = 3;
		static const int CAMERA_TRA_Y_DEC = 4;
		static const int CAMERA_TRA_X_INC = 5;
		static const int CAMERA_TRA_X_DEC = 6;
		static const int CAMERA_ROT_XY = 7;
		static const int CAMERA_ROT_XZ = 8;
		static const int SHOW_VERTEX_IDS = 9;
		static const int SHOW_POLYGON_IDS = 10;
		static const int SHOW_POLYHEDRON_IDS = 11;
		static const int SHOW_SELECT = 12;
		static const int SHOW_DELETE_RENDER = 12;
	private:
		UserInputTranslator();
		static UserInputTranslator* instance;
		std::map<std::string,std::vector<UserInputTranslationUnit> > keysDictionary;
};

#endif // USERINPUTTRANSLATOR_H
