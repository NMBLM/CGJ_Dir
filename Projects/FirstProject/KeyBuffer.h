#pragma once
#ifndef KEYBUFFER_H
#define KEYBUFFER_H



class KeyBuffer {
private:
	bool* keyStates = new bool[256];
	static KeyBuffer* buffer;
	KeyBuffer();

public:
	static KeyBuffer* instance();
	void pressKey(unsigned char key);
	void releaseKey(unsigned char key);
	bool isKeyDown(unsigned char key);



	
};

#endif