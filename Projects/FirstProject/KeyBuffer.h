#pragma once
#ifndef KEYBUFFER_H
#define KEYBUFFER_H

#include <string>
#include <map>


class KeyBuffer{
	private:
	std::map<unsigned char, bool> keyStates = std::map<unsigned char, bool>();
	std::map<int, bool> specialKeyStates = std::map<int, bool>();

	static KeyBuffer* buffer;
	KeyBuffer();

	public:
	static KeyBuffer* instance();
	void pressKey( unsigned char key );
	void releaseKey( unsigned char key );
	bool isKeyDown( unsigned char key );
	void pressSpecialKey( int key );
	void releaseSpecialKey( int key );
	bool isSpecialKeyDown( int key );

};

#endif