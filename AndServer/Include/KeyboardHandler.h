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

#ifndef KEYBOARDHANDLER_H_INCLUDED
#define KEYBOARDHANDLER_H_INCLUDED

#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <syslog.h>
#include <errno.h>
#include <sys/stat.h>

#include <iostream>
using namespace std;

#include "keymap.h"
#include "Logger.h"

#define KEYBOARD_USLEEP_TIME 7500

class KeyboardHandler {
	public:
		KeyboardHandler(const char* keyboardFilePathArg, class Logger *loggerArg);
		~KeyboardHandler();
		bool openKeyboard();
		bool closeKeyboard();
		bool sendKeystroke(const unsigned char key, const int value);
		void sendKey(const char key);
	private:

		int keyboardFd;
		char *keyboardFilePath;
		class Logger *logger;
};

#endif

