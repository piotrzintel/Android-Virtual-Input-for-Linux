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

#ifndef MOUSEHANDLER_H_INCLUDED
#define MOUSEHANDLER_H_INCLUDED

#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <syslog.h>
#include <errno.h>
#include <sys/stat.h>

#include <iostream>
using namespace std;

#include "Logger.h"

#define EV_MOVE 0
#define EV_BTN_LEFT_PRESS 1
#define EV_BTN_LEFT_RELEASE 2
#define EV_BTN_RIGHT_PRESS 3
#define EV_BTN_RIGHT_RELEASE 4
#define EV_BTN_MIDDLE_PRESS 5
#define EV_BTN_MIDDLE_RELEASE 6
#define EV_SCROLL_HORIZ 7
#define EV_SCROLL_VERT 8

#define MOUSE_USLEEP_TIME 5000
#define MAXBUF 1024

class MouseHandler {
	public:
		MouseHandler(const char* mouseFilePathArg, class Logger *loggerArg);
		~MouseHandler();
		bool openMouse();
		bool closeMouse();
		bool sendMouseEvent(const unsigned char type = 0, const int Xvalue = 0, const int Yvalue = 0);
		void sendMouseClick(const unsigned char button = 3);
		void sendMouseMove(const int Xvalue = 0, const int Yvalue = 0);
		void sendMouseBtnPress(const unsigned char button);
		void sendMouseBtnRelease(const unsigned char button);
		void sendMouseHScroll(const int Xvalue = 0);
		void sendMouseVScroll(const int Xvalue = 0);

	private:
		int mouseFd;
		char *mouseFilePath;
		class Logger *logger;
};

#endif

