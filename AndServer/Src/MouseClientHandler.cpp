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

#include "MouseClientHandler.h"

MouseClientHandler::MouseClientHandler(const int connectionSocketArg, class Logger *loggerArg, char* mouseSemName, char* sslCertificateFileArg, 
						char* sslPrivateKeyFileArg, char* sslClientCertificateFileArg, char* mouseFilePathArg, bool verifyPeerCertificateArg)
	: ClientHandler( connectionSocketArg, loggerArg, mouseSemName, sslCertificateFileArg, sslPrivateKeyFileArg, sslClientCertificateFileArg, mouseFilePathArg, verifyPeerCertificateArg ) {
	mouseHandler = NULL;
}

MouseClientHandler::~MouseClientHandler() {
	if (mouseHandler) {
		mouseHandler->closeMouse();
		delete mouseHandler;
	}
}

bool MouseClientHandler::handleClient() {

	int *optval = new int(1);
	if (setsockopt(connectionSocket,SOL_SOCKET,SO_KEEPALIVE,optval, sizeof(int)) == -1) {
		logger->error("02.03.2012 17:14:53 setsockopt() error",errno);
		delete optval;
		return false;
	}

	if (setsockopt(connectionSocket,SOL_TCP,TCP_KEEPCNT,optval, sizeof(int)) == -1) {
		logger->error("02.03.2012 17:24:57 setsockopt() error",errno);
		delete optval;
		return false;
	}

	*optval = 60;
	if (setsockopt(connectionSocket,SOL_TCP,TCP_KEEPIDLE,optval, sizeof(int)) == -1) {
		logger->error("02.03.2012 17:24:57 setsockopt() error",errno);
		delete optval;
		return false;
	}
	delete optval;

	if (!sslInit()) {
		logger->error("17.12.2011 03:48:27 sslInit() error");
		return false;
	}

	if ( !readyDeviceHandler() ) {
		logger->error("11.12.2011 22:49:53 Could not ready MouseHandler");
		return false;
	}
	char buffer[sizeof(char)+sizeof(int)];
	unsigned char msgType;
	int msgLength;
	struct pollfd pollSock;

	pollSock.fd = connectionSocket;
	pollSock.events = (0 | POLLIN | POLLRDHUP); // there is input data or connection closed
	pollSock.revents = 0;

	if (!sendReady()) {
		logger->error("11.12.2011 23:25:02 sendReady() error");
		return true;
	}

	errno = 0;
	if ( (setpriority(PRIO_PROCESS,PRIO_PROCESS,-19) < -1) || ( errno != 0 ) ) {
		logger->error("setpriority() errror",errno);
	}

	struct sched_param sp;
	sp.sched_priority = 50;

	errno = 0;
	if ( (sched_setscheduler(0,SCHED_RR,&sp) < -1) || ( errno != 0 ) ) {
		logger->error("sched_setscheduler() errror",errno);
	}

	int errorCounter = 0;
	while (!receivedEndSignal) {

		int ret = poll(&pollSock,1,900000);
		if (ret == 0) {
			logger->error("21.12.2011 14:12:10 Client timeout");
			break;
		} else if (ret == -1) {
			if (errno != EINTR) {
				logger->error("21.12.2011 14:12:05 poll() error",errno);
			}
			break;
		}
		if ( (pollSock.revents & POLLRDHUP) == POLLRDHUP ) {
			logger->printMessage("10.12.2011 23:59:58 Mouse disconnected");
			break;
		}

		int readBytes = 0;
		int retVal = 0;
		while ( readBytes != sizeof(char)+sizeof(int)) {
			if (receivedEndSignal) {
				return true;
			}
			if ( (retVal = SSL_read(ssl, buffer+readBytes, 5)) == -1) {
				logger->error("21.12.2011 02:24:02 read() error",errno);
				return true;
			}
			readBytes += retVal;
		}
		memcpy(&msgType, buffer, sizeof(char));
		memcpy(&msgLength, buffer+sizeof(char), sizeof(int));
		msgLength = ntohl(msgLength);

		char message[msgLength];
		readBytes = 0;
		retVal = 0;
		while (readBytes != msgLength) {
			if (receivedEndSignal) {
				return true;
			}
			if ( (retVal = SSL_read(ssl, message+readBytes, msgLength-readBytes )) == -1 ) {
				logger->error("21.12.2011 02:25:03 read() error",errno);
				return true;
			}
			readBytes += retVal;
		}
		if (readBytes != msgLength) {
			char tmp[128];
			sprintf(tmp,"20.12.2011 23:47:10 could not read message (read %d bytes of expected %d). Client timeout.",readBytes,msgLength);
			logger->error(tmp);
			break;
		}

		switch (msgType) {
			case MOUSE_MOVE:
				sem_wait(semaphore);
				receiveMouseMove(msgLength, message);
				sem_post(semaphore);
			break;

			case MOUSE_LEFT_BTN_PRESS:
				sem_wait(semaphore);
				mouseHandler->sendMouseBtnPress(0);
				sem_post(semaphore);
			break;

			case MOUSE_LEFT_BTN_RELEASE:
				sem_wait(semaphore);
				mouseHandler->sendMouseBtnRelease(0);
				sem_post(semaphore);
			break;

			case MOUSE_RIGHT_BTN_PRESS:
				sem_wait(semaphore);
				mouseHandler->sendMouseBtnPress(1);
				sem_post(semaphore);
			break;

			case MOUSE_RIGHT_BTN_RELEASE:
				sem_wait(semaphore);
				mouseHandler->sendMouseBtnRelease(1);
				sem_post(semaphore);
			break;

			case MOUSE_MIDDLE_BTN_PRESS:
				sem_wait(semaphore);
				mouseHandler->sendMouseBtnPress(2);
				sem_post(semaphore);
			break;

			case MOUSE_MIDDLE_BTN_RELEASE:
				sem_wait(semaphore);
				mouseHandler->sendMouseBtnRelease(2);
				sem_post(semaphore);
			break;

			case SCROLL_VERT:
				sem_wait(semaphore);
				receiveMouseVScroll(msgLength, message);
				sem_post(semaphore);
			break;

			case SCROLL_HORIZ:
				sem_wait(semaphore);
				receiveMouseHScroll(msgLength, message);
				sem_post(semaphore);
			break;

			case MSG_POLL:
			break;

			default:
				char tmp[128];
				sprintf(tmp,"21.12.2011 02:26:27 unknown message type: %u",(unsigned int)msgType);
				logger->error(tmp);
				if (++errorCounter >= 10) break;
			break;
		}
	}
	return true;
}

bool MouseClientHandler::receiveMouseMove(const int msgLength, const char* message) {
	if (msgLength == 2*sizeof(int)) {
		int Xvalue, Yvalue;
		memcpy(&Xvalue,message,sizeof(int));
		memcpy(&Yvalue,message+sizeof(int),sizeof(int));
		Xvalue = ntohl(Xvalue);
		Yvalue = ntohl(Yvalue);
		mouseHandler->sendMouseMove(Xvalue,Yvalue);
		return true;
	} else {
		logger->error("21.12.2011 02:26:16 Wrong message length");
		return true;
	}
}

bool MouseClientHandler::receiveMouseVScroll(const int msgLength, const char* message) {
	if (msgLength == sizeof(int)) {
		int Xvalue;
		memcpy(&Xvalue,message,sizeof(int));
		Xvalue = ntohl(Xvalue);
		mouseHandler->sendMouseVScroll(Xvalue);
		return true;
	} else {
		logger->error("10.03.2012 14:21:56 Wrong message length");
		return true;
	}
}

bool MouseClientHandler::receiveMouseHScroll(const int msgLength, const char* message) {
	if (msgLength == sizeof(int)) {
		int Xvalue;
		memcpy(&Xvalue,message,sizeof(int));
		Xvalue = ntohl(Xvalue);
		mouseHandler->sendMouseHScroll(Xvalue);
		return true;
	} else {
		logger->error("10.03.2012 14:22:27 Wrong message length");
		return true;
	}
}

bool MouseClientHandler::readyDeviceHandler(){

	if (!(mouseHandler = new MouseHandler(deviceFilePath,logger)) ) {
		logger->error("06.12.2011 01:34:22 error creating mouseHandler");
		logger->error("readyHandlers() failed");
		return false;
	}

	if (!mouseHandler->openMouse()) {
		logger->error("07.12.2011 15:27:33 error opening mouse");
		logger->error("readyHandlers() failed");
		return false;
	}

	return true;
}

