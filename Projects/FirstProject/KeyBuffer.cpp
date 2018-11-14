#include "KeyBuffer.h"



KeyBuffer* KeyBuffer::buffer = 0;

KeyBuffer::KeyBuffer() {

}

KeyBuffer * KeyBuffer::instance()
{
	if (buffer == 0) {
		buffer = new KeyBuffer();
	}
	return buffer;
}

void KeyBuffer::pressKey(unsigned char key)
{
	keyStates.insert(std::make_pair(key, true));
	keyStates.find(key)->second = true;
}

void KeyBuffer::releaseKey(unsigned char key)
{
	keyStates.insert(std::make_pair(key, false));
	keyStates.find(key)->second = false;

}

bool KeyBuffer::isKeyDown(unsigned char key)
{
	keyStates.insert(std::make_pair(key, false));

	return keyStates.find(key)->second;
}


void KeyBuffer::pressSpecialKey(int key)
{
	specialKeyStates.insert(std::make_pair(key, true));
	specialKeyStates.find(key)->second = true;
}

void KeyBuffer::releaseSpecialKey(int key)
{
	specialKeyStates.insert(std::make_pair(key, false));
	specialKeyStates.find(key)->second = false;

}

bool KeyBuffer::isSpecialKeyDown(int key)
{
	specialKeyStates.insert(std::make_pair(key, false));

	return specialKeyStates.find(key)->second;
}
