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

#include "MouseHandler.h"

MouseHandler::MouseHandler(const char* mouseFilePathArg, class Logger *loggerArg) {
	mouseFd = 0;
	mouseFilePath = new char[strlen(mouseFilePathArg) + 1];;
	strcpy(mouseFilePath,mouseFilePathArg);
	logger = loggerArg;
}

MouseHandler::~MouseHandler(){
	delete[] mouseFilePath;
}

bool MouseHandler::openMouse(){
	struct stat st;
	if ( stat(mouseFilePath, &st) != 0 ) {
		dev_t dev = makedev(60, 0);
		if ( mknod(mouseFilePath, S_IFCHR | S_IWUSR, dev) == -1 ) {
			logger->error("03.03.2012 14:13:16 openMouse: Could not create a mouse device file",errno);
			return false;
		}
	}

	if ( (mouseFd = open(mouseFilePath, O_WRONLY)) == -1 ) {
		logger->error("11.12.2011 21:53:45 openMouse() error: could not open mouse file",errno);
		return false;
	}
	return true;
}

bool MouseHandler::closeMouse() {
	if (mouseFd != 0 && mouseFd != -1) {
		if ( close(mouseFd) != 0 ) {
			char tmp[128];
			sprintf(tmp,"11.12.2011 22:16:23 close() mouse error: (%d) %s",errno, strerror(errno));
			logger->error(tmp);
			return false;
		}
	}
	return true;
}

bool MouseHandler::sendMouseEvent(const unsigned char type, const int Xvalue, const int Yvalue){
	char * writeBuffer = (char *)::operator new(sizeof(char)+2*sizeof(int));
	memcpy(writeBuffer,&type,sizeof(char));
	memcpy(writeBuffer+sizeof(char),&Xvalue,sizeof(int));
	memcpy(writeBuffer+sizeof(char)+sizeof(int),&Yvalue,sizeof(int));
	if ( write(mouseFd,writeBuffer,sizeof(char)+2*sizeof(int)) != (sizeof(char)+2*sizeof(int)) ) {
		logger->error("11.12.2011 21:53:54 sendMouseEvent write() error");
		delete writeBuffer;
		return false;
	}
	delete writeBuffer;
	return true;
}

void MouseHandler::sendMouseClick(const unsigned char button) {
	switch (button) {
	case 0:
		if (!sendMouseEvent(EV_BTN_LEFT_PRESS)) {
			logger->error("11.12.2011 21:54:01 sendMouseEvent(EV_BTN_LEFT_PRESS) error");
		}
		if (!sendMouseEvent(EV_BTN_LEFT_RELEASE)) {
			logger->error("11.12.2011 21:54:07 sendMouseEvent(EV_BTN_LEFT_RELEASE) error");
		}
	break;

	case 1:
		if (!sendMouseEvent(EV_BTN_RIGHT_PRESS)) {
			logger->error("11.12.2011 21:54:13 sendMouseEvent(EV_BTN_RIGHT_PRESS) error");
		}
		if (!sendMouseEvent(EV_BTN_RIGHT_RELEASE)) {
			logger->error("11.12.2011 21:54:20 sendMouseEvent(EV_BTN_RIGHT_RELEASE) error");
		}
	break;

	case 2:
		if (!sendMouseEvent(EV_BTN_MIDDLE_PRESS)) {
			logger->error("11.12.2011 21:54:25 sendMouseEvent(EV_BTN_MIDDLE_PRESS) error");
		}
		if (!sendMouseEvent(EV_BTN_MIDDLE_RELEASE)) {
			logger->error("11.12.2011 21:54:31 sendMouseEvent(EV_BTN_MIDDLE_RELEASE) error");
		}
	break;

	default:
		char tmp[128];
		sprintf(tmp,"11.12.2011 21:54:38 sendMouseClick - unknown button mouse (%u)",(unsigned int)button);
		logger->error(tmp);
	}
}

void MouseHandler::sendMouseMove(const int Xvalue, const int Yvalue) {
	if (!sendMouseEvent(EV_MOVE,Xvalue,Yvalue)) {
		logger->error("11.12.2011 21:54:44 sendMouseMove() error");
	}
}

void MouseHandler::sendMouseBtnPress(const unsigned char button) {
	switch (button) {
	case 0:
		if (!sendMouseEvent(EV_BTN_LEFT_PRESS)) {
			logger->error("12.12.2011 01:24:21 sendMouseEvent(EV_BTN_LEFT_PRESS) error");
		}
	break;

	case 1:
		if (!sendMouseEvent(EV_BTN_RIGHT_PRESS)) {
			logger->error("12.12.2011 01:24:26 sendMouseEvent(EV_BTN_RIGHT_PRESS) error");
		}
	break;

	case 2:
		if (!sendMouseEvent(EV_BTN_MIDDLE_PRESS)) {
			logger->error("12.12.2011 01:24:31 sendMouseEvent(EV_BTN_MIDDLE_PRESS) error");
		}
	break;

	default:
		char tmp[128];
		sprintf(tmp,"12.12.2011 01:24:40 sendMouseBtnPress - unknown button mouse (%u)",(unsigned int)button);
		logger->error(tmp);
	}
}

void MouseHandler::sendMouseBtnRelease(const unsigned char button) {
	switch (button) {
	case 0:
		if (!sendMouseEvent(EV_BTN_LEFT_RELEASE)) {
			logger->error("12.12.2011 01:26:14 sendMouseEvent(EV_BTN_LEFT_RELEASE) error");
		}
	break;

	case 1:
		if (!sendMouseEvent(EV_BTN_RIGHT_RELEASE)) {
			logger->error("12.12.2011 01:26:20 sendMouseEvent(EV_BTN_RIGHT_RELEASE) error");
		}
	break;

	case 2:
		if (!sendMouseEvent(EV_BTN_MIDDLE_RELEASE)) {
			logger->error("12.12.2011 01:26:26 sendMouseEvent(EV_BTN_MIDDLE_RELEASE) error");
		}
	break;

	default:
		char tmp[128];
		sprintf(tmp,"12.12.2011 01:26:44 sendMouseBtnRelease - unknown button mouse (%u)",(unsigned int)button);
		logger->error(tmp);
	}
}

void MouseHandler::sendMouseHScroll(const int Xvalue) {
	if (!sendMouseEvent(EV_SCROLL_HORIZ,Xvalue)) {
		cerr << "sendMouseHScroll() error";
	}
}

void MouseHandler::sendMouseVScroll(const int Xvalue) {
	if (!sendMouseEvent(EV_SCROLL_VERT,Xvalue)) {
		cerr << "sendMouseVScroll() error";
	}
}

