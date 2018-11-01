#include "KeyBuffer.h"



KeyBuffer* KeyBuffer::buffer = 0;

KeyBuffer::KeyBuffer() {
	for (int i = 0; i < 256; i++) {
		keyStates[i] = false;
	}
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
	keyStates[key] = true;
}

void KeyBuffer::releaseKey(unsigned char key)
{
	keyStates[key] = false;

}

bool KeyBuffer::isKeyDown(unsigned char key)
{
	return 	keyStates[key];
}
