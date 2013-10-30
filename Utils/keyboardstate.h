#ifndef KEYBOARDSTATE_H
#define KEYBOARDSTATE_H

#include <unordered_map>

class KeyboardState
{
	public:
		KeyboardState();
		void keyPressed(int key);
		void keyRelesed(int key);
		bool isKeyPressed(int key);
	private:
		std::unordered_map<int,bool> keymap;
};

#endif // KEYBOARDSTATE_H
