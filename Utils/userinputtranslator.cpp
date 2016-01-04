#include "Utils/userinputtranslator.h"
#include <map>
#include <Qt>
UserInputTranslator* UserInputTranslator::instance = (UserInputTranslator*)0;

//#include <iostream>
UserInputTranslator::UserInputTranslator()
{
	ADD_USER_INPUT_DIC_UNIT("Camera","Move Y-",
							CAMERA_TRA_Y_DEC,Qt::Key_Up,false,false);

	ADD_USER_INPUT_DIC_UNIT("Camera","Move Y+",
							CAMERA_TRA_Y_INC,Qt::Key_Down,false,false);

	ADD_USER_INPUT_DIC_UNIT("Camera","Move X-",
							CAMERA_TRA_X_DEC,Qt::Key_Left,false,false);

	ADD_USER_INPUT_DIC_UNIT("Camera","Move X+",
							CAMERA_TRA_X_INC,Qt::Key_Right,false,false);

	ADD_USER_INPUT_DIC_UNIT("Camera","Move Z-",
							CAMERA_TRA_Z_DEC,Qt::Key_Down,false,true);

	ADD_USER_INPUT_DIC_UNIT("Camera","Move Z+",
							CAMERA_TRA_Z_INC,Qt::Key_Up,false,true);

	ADD_USER_INPUT_DIC_UNIT("Camera","Rotate XY",
							CAMERA_ROT_XY,Qt::LeftButton,false,false);

	ADD_USER_INPUT_DIC_UNIT("Camera","Rotate XZ",
							CAMERA_ROT_XZ,Qt::RightButton,false,false);
	ADD_USER_INPUT_DIC_UNIT("Camera","Zoom +",
							CAMERA_ROT_XZ,
							UserInputTranslationUnit::UP_MOUSE_WHEEL,
							false,false);
	ADD_USER_INPUT_DIC_UNIT("Camera","Zoom -",
							CAMERA_ROT_XZ,
							UserInputTranslationUnit::DOWN_MOUSE_WHEEL,
							false,false);

	ADD_USER_INPUT_DIC_UNIT("Elements Ids","Show Vertex Ids",
							SHOW_VERTEX_IDS,Qt::Key_U,false,false);

	ADD_USER_INPUT_DIC_UNIT("Elements Ids","Show Polygon Ids",
							SHOW_POLYGON_IDS,Qt::Key_I,false,false);

	ADD_USER_INPUT_DIC_UNIT("Elements Ids","Show Polyhedron Ids",
							SHOW_POLYHEDRON_IDS,Qt::Key_O,false,false);

	ADD_USER_INPUT_DIC_UNIT("Others","Select/Unselect",
							SHOW_SELECT,Qt::LeftButton,true,false);

	ADD_USER_INPUT_DIC_UNIT("Others","Delete render",
							SHOW_DELETE_RENDER,Qt::Key_Delete,false,false);

}

UserInputTranslator* UserInputTranslator::getSingletonInstance(){
	if(!instance)
		instance = new UserInputTranslator();
	return instance;
}

void UserInputTranslator::deleteSingletonInstance(){
	if(instance){
		delete instance;
		instance = 0;
	}
}

int UserInputTranslator::getTranslatedAction(KeyboardState& kstate){
	typedef std::map<std::string,std::vector<UserInputTranslationUnit> >::iterator it_type;
	for(it_type iterator = keysDictionary.begin();
		iterator != keysDictionary.end(); iterator++) {
		std::vector<UserInputTranslationUnit>& units = iterator->second;
		for( UserInputTranslationUnit& unit : units ){
			if(unit.isItComplete(kstate))
				return unit.getCode();
		}
	}
	return NULL_ACTION;
}

bool UserInputTranslator::getConfigFromFile(std::string filename){
	return true;
}

bool UserInputTranslator::saveConfigToFile(std::string filename){
	return true;
}

std::map<std::string,std::vector<UserInputTranslationUnit> >& UserInputTranslator::getDictionary(){
	return keysDictionary;
}
