/*

   AndServer, part of Android™ Virtual Input for Linux project

   Copyright 2012 Piotr Zintel
   zintelpiotr@gmail.com

   Android is a trademark of Google Inc.

*/

/*

   AndServer is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   AndServer is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "KeyboardHandler.h"

KeyboardHandler::KeyboardHandler(const char* keyboardFilePathArg, class Logger *loggerArg) {
	keyboardFd = 0;
	keyboardFilePath = new char[strlen(keyboardFilePathArg) + 1];
	strcpy(keyboardFilePath,keyboardFilePathArg);
	logger = loggerArg;
}

KeyboardHandler::~KeyboardHandler(){
	delete[] keyboardFilePath;
}

bool KeyboardHandler::openKeyboard() {
	struct stat st;
	if ( stat(keyboardFilePath, &st) != 0 ) {
		dev_t dev = makedev(61, 0);
		if ( mknod(keyboardFilePath, S_IFCHR | S_IWUSR, dev) == -1 ) {
			logger->error("03.03.2012 14:09:24 error: Could not create a keyboard device file",errno);
			return false;
		}
	}
	if ( (keyboardFd = open(keyboardFilePath, O_WRONLY)) == -1 ) {
		logger->error("11.12.2011 21:52:45 error: Could not open keyboard device file",errno);
		return false;
	}
	return true;
}

bool KeyboardHandler::closeKeyboard() {
	if (keyboardFd != 0 && keyboardFd != -1) {
		if ( close(keyboardFd) != 0 ) {
			char tmp[128];
			sprintf(tmp,"11.12.2011 21:47:59 error: (%d) %s",errno, strerror(errno));
			logger->error(tmp);
			return false;
		}
	}
	return true;
}

bool KeyboardHandler::sendKeystroke(const unsigned char key, const int value) {
	char buffer[2];
	if ( (value % 2) != value ) {
		logger->error("11.12.2011 21:48:45 error: value not 0 nor 1");
		return false;
	}
	if ( (key % (AVKBD_KEYMAP_SIZE+1)) != key ) {
		char tmp[128];
		sprintf(tmp,"11.12.2011 21:50:45 error: unknown scanCode: %u",(unsigned int)key);
		logger->error(tmp);
		return false;
	}
	buffer[0]=key;
	buffer[1]=value;
	if ( write(keyboardFd, buffer, 2*sizeof(char)) != 2*sizeof(char) ) {
		logger->error("11.12.2011 21:49:30 error: write() error");
		return false;
	}
	usleep(KEYBOARD_USLEEP_TIME);
	return true;
}

void KeyboardHandler::sendKey(const char key) {
	switch (key) {
		case ' ':
			if(!sendKeystroke(AVKBD_KEY_SPACE,1)) {
				logger->error("AVKBD_KEY_SPACE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_SPACE,0)) {
				logger->error("AVKBD_KEY_SPACE,0");
			}
		break;

		case '1':
			if(!sendKeystroke(AVKBD_KEY_1,1)) {
				logger->error("AVKBD_KEY_1,1");
			}
			if(!sendKeystroke(AVKBD_KEY_1,0)) {
				logger->error("AVKBD_KEY_1,0");
			}
		break;

		case '2':
			if(!sendKeystroke(AVKBD_KEY_2,1)) {
				logger->error("AVKBD_KEY_2,1");
			}
			if(!sendKeystroke(AVKBD_KEY_2,0)) {
				logger->error("AVKBD_KEY_2,0");
			}
		break;

		case '3':
			if(!sendKeystroke(AVKBD_KEY_3,1)) {
				logger->error("AVKBD_KEY_3,1");
			}
			if(!sendKeystroke(AVKBD_KEY_3,0)) {
				logger->error("AVKBD_KEY_3,0");
			}
		break;

		case '4':
			if(!sendKeystroke(AVKBD_KEY_4,1)) {
				logger->error("AVKBD_KEY_4,1");
			}
			if(!sendKeystroke(AVKBD_KEY_4,0)) {
				logger->error("AVKBD_KEY_4,0");
			}
		break;

		case '5':
			if(!sendKeystroke(AVKBD_KEY_5,1)) {
				logger->error("AVKBD_KEY_5,1");
			}
			if(!sendKeystroke(AVKBD_KEY_5,0)) {
				logger->error("AVKBD_KEY_5,0");
			}
		break;

		case '6':
			if(!sendKeystroke(AVKBD_KEY_6,1)) {
				logger->error("AVKBD_KEY_6,1");
			}
			if(!sendKeystroke(AVKBD_KEY_6,0)) {
				logger->error("AVKBD_KEY_6,0");
			}
		break;

		case '7':
			if(!sendKeystroke(AVKBD_KEY_7,1)) {
				logger->error("AVKBD_KEY_7,1");
			}
			if(!sendKeystroke(AVKBD_KEY_7,0)) {
				logger->error("AVKBD_KEY_7,0");
			}
		break;

		case '8':
			if(!sendKeystroke(AVKBD_KEY_8,1)) {
				logger->error("AVKBD_KEY_8,1");
			}
			if(!sendKeystroke(AVKBD_KEY_8,0)) {
				logger->error("AVKBD_KEY_8,0");
			}
		break;

		case '9':
			if(!sendKeystroke(AVKBD_KEY_9,1)) {
				logger->error("AVKBD_KEY_9,1");
			}
			if(!sendKeystroke(AVKBD_KEY_9,0)) {
				logger->error("AVKBD_KEY_9,0");
			}
		break;

		case '0':
			if(!sendKeystroke(AVKBD_KEY_0,1)) {
				logger->error("AVKBD_KEY_0,1");
			}
			if(!sendKeystroke(AVKBD_KEY_0,0)) {
				logger->error("AVKBD_KEY_0,0");
			}
		break;

		case 'q':
			if(!sendKeystroke(AVKBD_KEY_Q,1)) {
				logger->error("AVKBD_KEY_Q,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Q,0)) {
				logger->error("AVKBD_KEY_Q,0");
			}
		break;

		case 'w':
			if(!sendKeystroke(AVKBD_KEY_W,1)) {
				logger->error("AVKBD_KEY_W,1");
			}
			if(!sendKeystroke(AVKBD_KEY_W,0)) {
				logger->error("AVKBD_KEY_W,0");
			}
		break;

		case 'e':
			if(!sendKeystroke(AVKBD_KEY_E,1)) {
				logger->error("AVKBD_KEY_E,1");
			}
			if(!sendKeystroke(AVKBD_KEY_E,0)) {
				logger->error("AVKBD_KEY_E,0");
			}
		break;

		case 'r':
			if(!sendKeystroke(AVKBD_KEY_R,1)) {
				logger->error("AVKBD_KEY_R,1");
			}
			if(!sendKeystroke(AVKBD_KEY_R,0)) {
				logger->error("AVKBD_KEY_R,0");
			}
		break;

		case 't':
			if(!sendKeystroke(AVKBD_KEY_T,1)) {
				logger->error("AVKBD_KEY_T,1");
			}
			if(!sendKeystroke(AVKBD_KEY_T,0)) {
				logger->error("AVKBD_KEY_T,0");
			}
		break;

		case 'y':
			if(!sendKeystroke(AVKBD_KEY_Y,1)) {
				logger->error("AVKBD_KEY_Y,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Y,0)) {
				logger->error("AVKBD_KEY_Y,0");
			}
		break;

		case 'u':
			if(!sendKeystroke(AVKBD_KEY_U,1)) {
				logger->error("AVKBD_KEY_U,1");
			}
			if(!sendKeystroke(AVKBD_KEY_U,0)) {
				logger->error("AVKBD_KEY_U,0");
			}
		break;

		case 'i':
			if(!sendKeystroke(AVKBD_KEY_I,1)) {
				logger->error("AVKBD_KEY_I,1");
			}
			if(!sendKeystroke(AVKBD_KEY_I,0)) {
				logger->error("AVKBD_KEY_I,0");
			}
		break;

		case 'o':
			if(!sendKeystroke(AVKBD_KEY_O,1)) {
				logger->error("AVKBD_KEY_O,1");
			}
			if(!sendKeystroke(AVKBD_KEY_O,0)) {
				logger->error("AVKBD_KEY_O,0");
			}
		break;

		case 'p':
			if(!sendKeystroke(AVKBD_KEY_P,1)) {
				logger->error("AVKBD_KEY_P,1");
			}
			if(!sendKeystroke(AVKBD_KEY_P,0)) {
				logger->error("AVKBD_KEY_P,0");
			}
		break;

		case 'a':
			if(!sendKeystroke(AVKBD_KEY_A,1)) {
				logger->error("AVKBD_KEY_A,1");
			}
			if(!sendKeystroke(AVKBD_KEY_A,0)) {
				logger->error("AVKBD_KEY_A,0");
			}
		break;

		case 's':
			if(!sendKeystroke(AVKBD_KEY_S,1)) {
				logger->error("AVKBD_KEY_S,1");
			}
			if(!sendKeystroke(AVKBD_KEY_S,0)) {
				logger->error("AVKBD_KEY_S,0");
			}
		break;

		case 'd':
			if(!sendKeystroke(AVKBD_KEY_D,1)) {
				logger->error("AVKBD_KEY_D,1");
			}
			if(!sendKeystroke(AVKBD_KEY_D,0)) {
				logger->error("AVKBD_KEY_D,0");
			}
		break;

		case 'f':
			if(!sendKeystroke(AVKBD_KEY_F,1)) {
				logger->error("AVKBD_KEY_F,1");
			}
			if(!sendKeystroke(AVKBD_KEY_F,0)) {
				logger->error("AVKBD_KEY_F,0");
			}
		break;

		case 'g':
			if(!sendKeystroke(AVKBD_KEY_G,1)) {
				logger->error("AVKBD_KEY_G,1");
			}
			if(!sendKeystroke(AVKBD_KEY_G,0)) {
				logger->error("AVKBD_KEY_G,0");
			}
		break;

		case 'h':
			if(!sendKeystroke(AVKBD_KEY_H,1)) {
				logger->error("AVKBD_KEY_H,1");
			}
			if(!sendKeystroke(AVKBD_KEY_H,0)) {
				logger->error("AVKBD_KEY_H,0");
			}
		break;

		case 'j':
			if(!sendKeystroke(AVKBD_KEY_J,1)) {
				logger->error("AVKBD_KEY_J,1");
			}
			if(!sendKeystroke(AVKBD_KEY_J,0)) {
				logger->error("AVKBD_KEY_J,0");
			}
		break;

		case 'k':
			if(!sendKeystroke(AVKBD_KEY_K,1)) {
				logger->error("AVKBD_KEY_K,1");
			}
			if(!sendKeystroke(AVKBD_KEY_K,0)) {
				logger->error("AVKBD_KEY_K,0");
			}
		break;

		case 'l':
			if(!sendKeystroke(AVKBD_KEY_L,1)) {
				logger->error("AVKBD_KEY_L,1");
			}
			if(!sendKeystroke(AVKBD_KEY_L,0)) {
				logger->error("AVKBD_KEY_L,0");
			}
		break;

		case 'z':
			if(!sendKeystroke(AVKBD_KEY_Z,1)) {
				logger->error("AVKBD_KEY_Z,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Z,0)) {
				logger->error("AVKBD_KEY_Z,0");
			}
		break;

		case 'x':
			if(!sendKeystroke(AVKBD_KEY_X,1)) {
				logger->error("AVKBD_KEY_X,1");
			}
			if(!sendKeystroke(AVKBD_KEY_X,0)) {
				logger->error("AVKBD_KEY_X,0");
			}
		break;

		case 'c':
			if(!sendKeystroke(AVKBD_KEY_C,1)) {
				logger->error("AVKBD_KEY_C,1");
			}
			if(!sendKeystroke(AVKBD_KEY_C,0)) {
				logger->error("AVKBD_KEY_C,0");
			}
		break;

		case 'v':
			if(!sendKeystroke(AVKBD_KEY_V,1)) {
				logger->error("AVKBD_KEY_V,1");
			}
			if(!sendKeystroke(AVKBD_KEY_V,0)) {
				logger->error("AVKBD_KEY_V,0");
			}
		break;

		case 'b':
			if(!sendKeystroke(AVKBD_KEY_B,1)) {
				logger->error("AVKBD_KEY_B,1");
			}
			if(!sendKeystroke(AVKBD_KEY_B,0)) {
				logger->error("AVKBD_KEY_B,0");
			}
		break;

		case 'n':
			if(!sendKeystroke(AVKBD_KEY_N,1)) {
				logger->error("AVKBD_KEY_N,1");
			}
			if(!sendKeystroke(AVKBD_KEY_N,0)) {
				logger->error("AVKBD_KEY_N,0");
			}
		break;

		case 'm':
			if(!sendKeystroke(AVKBD_KEY_M,1)) {
				logger->error("AVKBD_KEY_M,1");
			}
			if(!sendKeystroke(AVKBD_KEY_M,0)) {
				logger->error("AVKBD_KEY_M,0");
			}
		break;

		case '-':
			if(!sendKeystroke(AVKBD_KEY_MINUS,1)) {
				logger->error("AVKBD_KEY_MINUS,1");
			}
			if(!sendKeystroke(AVKBD_KEY_MINUS,0)) {
				logger->error("AVKBD_KEY_MINUS,0");
			}
		break;

		case '=':
			if(!sendKeystroke(AVKBD_KEY_EQUAL,1)) {
				logger->error("AVKBD_KEY_EQUAL,1");
			}
			if(!sendKeystroke(AVKBD_KEY_EQUAL,0)) {
				logger->error("AVKBD_KEY_EQUAL,0");
			}
		break;

		case '[':
			if(!sendKeystroke(AVKBD_KEY_LEFTBRACE,1)) {
				logger->error("AVKBD_KEY_LEFTBRACE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTBRACE,0)) {
				logger->error("AVKBD_KEY_LEFTBRACE,0");
			}
		break;

		case ']':
			if(!sendKeystroke(AVKBD_KEY_RIGHTBRACE,1)) {
				logger->error("AVKBD_KEY_RIGHTBRACE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_RIGHTBRACE,0)) {
				logger->error("AVKBD_KEY_RIGHTBRACE,0");
			}
		break;

		case '\\':
			if(!sendKeystroke(AVKBD_KEY_BACKSLASH,1)) {
				logger->error("AVKBD_KEY_BACKSLASH,1");
			}
			if(!sendKeystroke(AVKBD_KEY_BACKSLASH,0)) {
				logger->error("AVKBD_KEY_BACKSLASH,0");
			}
		break;

		case ';':
			if(!sendKeystroke(AVKBD_KEY_SEMICOLON,1)) {
				logger->error("AVKBD_KEY_SEMICOLON,1");
			}
			if(!sendKeystroke(AVKBD_KEY_SEMICOLON,0)) {
				logger->error("AVKBD_KEY_SEMICOLON,0");
			}
		break;

		case '\'':
			if(!sendKeystroke(AVKBD_KEY_APOSTROPHE,1)) {
				logger->error("AVKBD_KEY_APOSTROPHE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_APOSTROPHE,0)) {
				logger->error("AVKBD_KEY_APOSTROPHE,0");
			}
		break;

		case ',':
			if(!sendKeystroke(AVKBD_KEY_COMMA,1)) {
				logger->error("AVKBD_KEY_COMMA,1");
			}
			if(!sendKeystroke(AVKBD_KEY_COMMA,0)) {
				logger->error("AVKBD_KEY_COMMA,0");
			}
		break;

		case '.':
			if(!sendKeystroke(AVKBD_KEY_DOT,1)) {
				logger->error("AVKBD_KEY_DOT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_DOT,0)) {
				logger->error("AVKBD_KEY_DOT,0");
			}
		break;

		case '/':
			if(!sendKeystroke(AVKBD_KEY_SLASH,1)) {
				logger->error("AVKBD_KEY_SLASH,1");
			}
			if(!sendKeystroke(AVKBD_KEY_SLASH,0)) {
				logger->error("AVKBD_KEY_SLASH,0");
			}
		break;

		case '`':
			if(!sendKeystroke(AVKBD_KEY_GRAVE,1)) {
				logger->error("AVKBD_KEY_GRAVE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_GRAVE,0)) {
				logger->error("AVKBD_KEY_GRAVE,0");
			}
		break;

		case 'A':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_A,1)) {
				logger->error("AVKBD_KEY_A,1");
			}
			if(!sendKeystroke(AVKBD_KEY_A,0)) {
				logger->error("AVKBD_KEY_A,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'B':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_B,1)) {
				logger->error("AVKBD_KEY_B,1");
			}
			if(!sendKeystroke(AVKBD_KEY_B,0)) {
				logger->error("AVKBD_KEY_B,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'C':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_C,1)) {
				logger->error("AVKBD_KEY_C,1");
			}
			if(!sendKeystroke(AVKBD_KEY_C,0)) {
				logger->error("AVKBD_KEY_C,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'D':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_D,1)) {
				logger->error("AVKBD_KEY_D,1");
			}
			if(!sendKeystroke(AVKBD_KEY_D,0)) {
				logger->error("AVKBD_KEY_D,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'E':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_E,1)) {
				logger->error("AVKBD_KEY_E,1");
			}
			if(!sendKeystroke(AVKBD_KEY_E,0)) {
				logger->error("AVKBD_KEY_E,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'F':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_F,1)) {
				logger->error("AVKBD_KEY_F,1");
			}
			if(!sendKeystroke(AVKBD_KEY_F,0)) {
				logger->error("AVKBD_KEY_F,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'G':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_G,1)) {
				logger->error("AVKBD_KEY_G,1");
			}
			if(!sendKeystroke(AVKBD_KEY_G,0)) {
				logger->error("AVKBD_KEY_G,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'H':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_H,1)) {
				logger->error("AVKBD_KEY_H,1");
			}
			if(!sendKeystroke(AVKBD_KEY_H,0)) {
				logger->error("AVKBD_KEY_H,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'I':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_I,1)) {
				logger->error("AVKBD_KEY_I,1");
			}
			if(!sendKeystroke(AVKBD_KEY_I,0)) {
				logger->error("AVKBD_KEY_I,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'J':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_J,1)) {
				logger->error("AVKBD_KEY_J,1");
			}
			if(!sendKeystroke(AVKBD_KEY_J,0)) {
				logger->error("AVKBD_KEY_J,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'K':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_K,1)) {
				logger->error("AVKBD_KEY_K,1");
			}
			if(!sendKeystroke(AVKBD_KEY_K,0)) {
				logger->error("AVKBD_KEY_K,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'L':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_L,1)) {
				logger->error("AVKBD_KEY_L,1");
			}
			if(!sendKeystroke(AVKBD_KEY_L,0)) {
				logger->error("AVKBD_KEY_L,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'M':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_M,1)) {
				logger->error("AVKBD_KEY_M,1");
			}
			if(!sendKeystroke(AVKBD_KEY_M,0)) {
				logger->error("AVKBD_KEY_M,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'N':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_N,1)) {
				logger->error("AVKBD_KEY_N,1");
			}
			if(!sendKeystroke(AVKBD_KEY_N,0)) {
				logger->error("AVKBD_KEY_N,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'O':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_O,1)) {
				logger->error("AVKBD_KEY_O,1");
			}
			if(!sendKeystroke(AVKBD_KEY_O,0)) {
				logger->error("AVKBD_KEY_O,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'P':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_P,1)) {
				logger->error("AVKBD_KEY_P,1");
			}
			if(!sendKeystroke(AVKBD_KEY_P,0)) {
				logger->error("AVKBD_KEY_P,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'Q':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Q,1)) {
				logger->error("AVKBD_KEY_Q,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Q,0)) {
				logger->error("AVKBD_KEY_Q,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'R':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_R,1)) {
				logger->error("AVKBD_KEY_R,1");
			}
			if(!sendKeystroke(AVKBD_KEY_R,0)) {
				logger->error("AVKBD_KEY_R,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'S':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_S,1)) {
				logger->error("AVKBD_KEY_S,1");
			}
			if(!sendKeystroke(AVKBD_KEY_S,0)) {
				logger->error("AVKBD_KEY_S,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'T':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_T,1)) {
				logger->error("AVKBD_KEY_T,1");
			}
			if(!sendKeystroke(AVKBD_KEY_T,0)) {
				logger->error("AVKBD_KEY_T,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'U':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_U,1)) {
				logger->error("AVKBD_KEY_U,1");
			}
			if(!sendKeystroke(AVKBD_KEY_U,0)) {
				logger->error("AVKBD_KEY_U,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'V':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_V,1)) {
				logger->error("AVKBD_KEY_V,1");
			}
			if(!sendKeystroke(AVKBD_KEY_V,0)) {
				logger->error("AVKBD_KEY_V,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'W':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_W,1)) {
				logger->error("AVKBD_KEY_W,1");
			}
			if(!sendKeystroke(AVKBD_KEY_W,0)) {
				logger->error("AVKBD_KEY_W,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'X':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_X,1)) {
				logger->error("AVKBD_KEY_X,1");
			}
			if(!sendKeystroke(AVKBD_KEY_X,0)) {
				logger->error("AVKBD_KEY_X,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'Y':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Y,1)) {
				logger->error("AVKBD_KEY_Y,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Y,0)) {
				logger->error("AVKBD_KEY_Y,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case 'Z':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Z,1)) {
				logger->error("AVKBD_KEY_Z,1");
			}
			if(!sendKeystroke(AVKBD_KEY_Z,0)) {
				logger->error("AVKBD_KEY_Z,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '!':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_1,1)) {
				logger->error("AVKBD_KEY_1,1");
			}
			if(!sendKeystroke(AVKBD_KEY_1,0)) {
				logger->error("AVKBD_KEY_1,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '@':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_2,1)) {
				logger->error("AVKBD_KEY_2,1");
			}
			if(!sendKeystroke(AVKBD_KEY_2,0)) {
				logger->error("AVKBD_KEY_2,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '#':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_3,1)) {
				logger->error("AVKBD_KEY_3,1");
			}
			if(!sendKeystroke(AVKBD_KEY_3,0)) {
				logger->error("AVKBD_KEY_3,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '$':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_4,1)) {
				logger->error("AVKBD_KEY_4,1");
			}
			if(!sendKeystroke(AVKBD_KEY_4,0)) {
				logger->error("AVKBD_KEY_4,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '%':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_5,1)) {
				logger->error("AVKBD_KEY_5,1");
			}
			if(!sendKeystroke(AVKBD_KEY_5,0)) {
				logger->error("AVKBD_KEY_5,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '^':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_6,1)) {
				logger->error("AVKBD_KEY_6,1");
			}
			if(!sendKeystroke(AVKBD_KEY_6,0)) {
				logger->error("AVKBD_KEY_6,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '&':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_7,1)) {
				logger->error("AVKBD_KEY_7,1");
			}
			if(!sendKeystroke(AVKBD_KEY_7,0)) {
				logger->error("AVKBD_KEY_7,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '*':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_8,1)) {
				logger->error("AVKBD_KEY_8,1");
			}
			if(!sendKeystroke(AVKBD_KEY_8,0)) {
				logger->error("AVKBD_KEY_8,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '(':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_9,1)) {
				logger->error("AVKBD_KEY_9,1");
			}
			if(!sendKeystroke(AVKBD_KEY_9,0)) {
				logger->error("AVKBD_KEY_9,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case ')':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_0,1)) {
				logger->error("AVKBD_KEY_0,1");
			}
			if(!sendKeystroke(AVKBD_KEY_0,0)) {
				logger->error("AVKBD_KEY_0,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '_':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_MINUS,1)) {
				logger->error("AVKBD_KEY_MINUS,1");
			}
			if(!sendKeystroke(AVKBD_KEY_MINUS,0)) {
				logger->error("AVKBD_KEY_MINUS,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '+':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_EQUAL,1)) {
				logger->error("AVKBD_KEY_EQUAL,1");
			}
			if(!sendKeystroke(AVKBD_KEY_EQUAL,0)) {
				logger->error("AVKBD_KEY_EQUAL,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '{':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTBRACE,1)) {
				logger->error("AVKBD_KEY_LEFTBRACE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTBRACE,0)) {
				logger->error("AVKBD_KEY_LEFTBRACE,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '}':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_RIGHTBRACE,1)) {
				logger->error("AVKBD_KEY_RIGHTBRACE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_RIGHTBRACE,0)) {
				logger->error("AVKBD_KEY_RIGHTBRACE,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '|':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_BACKSLASH,1)) {
				logger->error("AVKBD_KEY_BACKSLASH,1");
			}
			if(!sendKeystroke(AVKBD_KEY_BACKSLASH,0)) {
				logger->error("AVKBD_KEY_BACKSLASH,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case ':':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_SEMICOLON,1)) {
				logger->error("AVKBD_KEY_SEMICOLON,1");
			}
			if(!sendKeystroke(AVKBD_KEY_SEMICOLON,0)) {
				logger->error("AVKBD_KEY_SEMICOLON,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '"':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_APOSTROPHE,1)) {
				logger->error("AVKBD_KEY_APOSTROPHE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_APOSTROPHE,0)) {
				logger->error("AVKBD_KEY_APOSTROPHE,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '<':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_COMMA,1)) {
				logger->error("AVKBD_KEY_COMMA,1");
			}
			if(!sendKeystroke(AVKBD_KEY_COMMA,0)) {
				logger->error("AVKBD_KEY_COMMA,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '>':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_DOT,1)) {
				logger->error("AVKBD_KEY_DOT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_DOT,0)) {
				logger->error("AVKBD_KEY_DOT,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '?':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_SLASH,1)) {
				logger->error("AVKBD_KEY_SLASH,1");
			}
			if(!sendKeystroke(AVKBD_KEY_SLASH,0)) {
				logger->error("AVKBD_KEY_SLASH,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '~':
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,1)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,1");
			}
			if(!sendKeystroke(AVKBD_KEY_GRAVE,1)) {
				logger->error("AVKBD_KEY_GRAVE,1");
			}
			if(!sendKeystroke(AVKBD_KEY_GRAVE,0)) {
				logger->error("AVKBD_KEY_GRAVE,0");
			}
			if(!sendKeystroke(AVKBD_KEY_LEFTSHIFT,0)) {
				logger->error("AVKBD_KEY_LEFTSHIFT,0");
			}
		break;

		case '\n':
			if(!sendKeystroke(AVKBD_KEY_ENTER,1)) {
				logger->error("AVKBD_KEY_ENTER,1");
			}
			if(!sendKeystroke(AVKBD_KEY_ENTER,0)) {
				logger->error("AVKBD_KEY_ENTER,0");
			}
		break;

		default:
			char tmp[128];
			sprintf(tmp,"11.12.2011 21:51:34 error: unknown key: %d",(unsigned char)key);
			logger->error(tmp);
		break;
		};
}

