#include "keyboardstate.h"

KeyboardState::KeyboardState()
{
}

void KeyboardState::keyPressed(int key){
	keymap[key] = true;
}

void KeyboardState::keyRelesed(int key){
	keymap[key] = false;

}

bool KeyboardState::isKeyPressed(int key){
	return (keymap.count(key)>0)?keymap[key]:false;
}
