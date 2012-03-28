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

MouseClientHandler::MouseClientHandler(const int connectionSocketArg, class Logger *loggerArg, char* mouseSemName, char* sslCertificateFileArg, char* sslPrivateKeyFileArg, char* mouseFilePathArg) {
	sslCertificateFile = strcpy(new char[strlen(sslCertificateFileArg) + 1],sslCertificateFileArg);
	sslPrivateKeyFile = strcpy(new char[strlen(sslPrivateKeyFileArg) + 1],sslPrivateKeyFileArg);
	mouseFilePath = strcpy(new char[strlen(mouseFilePathArg) + 1],mouseFilePathArg);
	connectionSocket = connectionSocketArg;
	receivedEndSignal = false;
	receivedEndMessage = false;
	logger = loggerArg;
	ssl_method = NULL;
	ssl_ctx = NULL;	
	ssl = NULL;
	mouseHandler = NULL;
	if ((mouseSem = sem_open(mouseSemName,0,O_RDWR,0)) == SEM_FAILED) {
		logger->error("sem_open failed",errno);
	}
}

MouseClientHandler::~MouseClientHandler() {
	if (mouseHandler) {
		mouseHandler->closeMouse();
		delete mouseHandler;
	}
	sem_close(mouseSem);
	if (ssl) SSL_shutdown(ssl);
	if (ssl) SSL_free(ssl);
	if (ssl_ctx) SSL_CTX_free(ssl_ctx);
	if (sslCertificateFile) delete[] sslCertificateFile;
	if (sslPrivateKeyFile) delete[] sslPrivateKeyFile;
	if (mouseFilePath) delete[] mouseFilePath;
	close(connectionSocket);
}

bool MouseClientHandler::handleClient() {

	int *optval = new int(1);
	if (setsockopt(connectionSocket,SOL_SOCKET,SO_KEEPALIVE,optval, sizeof(int)) == -1) {
		char tmp[128];
		sprintf(tmp,"02.03.2012 17:14:53 setsockopt() error: (%d) %s",errno, strerror(errno));
		logger->error(tmp);
		delete optval;
		return false;
	}

	if (setsockopt(connectionSocket,SOL_TCP,TCP_KEEPCNT,optval, sizeof(int)) == -1) {
		char tmp[128];
		sprintf(tmp,"02.03.2012 17:24:57 setsockopt() error: (%d) %s",errno, strerror(errno));
		logger->error(tmp);
		delete optval;
		return false;
	}

	*optval = 60;
	if (setsockopt(connectionSocket,SOL_TCP,TCP_KEEPIDLE,optval, sizeof(int)) == -1) {
		char tmp[128];
		sprintf(tmp,"02.03.2012 17:24:57 setsockopt() error: (%d) %s",errno, strerror(errno));
		logger->error(tmp);
		delete optval;
		return false;
	}
	delete optval;

	if (!sslInit()) {
		logger->error("17.12.2011 03:48:27 sslInit() error");
		return false;
	}

	if ( !readyMouseHandler() ) {
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
				char tmp[128];
				sprintf(tmp,"21.12.2011 14:12:05 poll() error: (%d) %s",errno,strerror(errno));
				logger->error(tmp);
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
				sem_wait(mouseSem);
				receiveMouseMove(msgLength, message);
				sem_post(mouseSem);
			break;

			case MOUSE_LEFT_BTN_PRESS:
				sem_wait(mouseSem);
				mouseHandler->sendMouseBtnPress(0);
				sem_post(mouseSem);
			break;

			case MOUSE_LEFT_BTN_RELEASE:
				sem_wait(mouseSem);
				mouseHandler->sendMouseBtnRelease(0);
				sem_post(mouseSem);
			break;

			case MOUSE_RIGHT_BTN_PRESS:
				sem_wait(mouseSem);
				mouseHandler->sendMouseBtnPress(1);
				sem_post(mouseSem);
			break;

			case MOUSE_RIGHT_BTN_RELEASE:
				sem_wait(mouseSem);
				mouseHandler->sendMouseBtnRelease(1);
				sem_post(mouseSem);
			break;

			case MOUSE_MIDDLE_BTN_PRESS:
				sem_wait(mouseSem);
				mouseHandler->sendMouseBtnPress(2);
				sem_post(mouseSem);
			break;

			case MOUSE_MIDDLE_BTN_RELEASE:
				sem_wait(mouseSem);
				mouseHandler->sendMouseBtnRelease(2);
				sem_post(mouseSem);
			break;

			case SCROLL_VERT:
				sem_wait(mouseSem);
				receiveMouseVScroll(msgLength, message);
				sem_post(mouseSem);
			break;

			case SCROLL_HORIZ:
				sem_wait(mouseSem);
				receiveMouseHScroll(msgLength, message);
				sem_post(mouseSem);
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

bool MouseClientHandler::sslInit() {
	int ret = 0;
	OpenSSL_add_all_algorithms();
	SSL_library_init() ;
	SSL_load_error_strings();

	if ( (ssl_method = TLSv1_server_method()) == NULL ) {
		errError("17.12.2011 03:44:41 TLSv1_server_method() error");
		return false;
	}

	if((ssl_ctx = SSL_CTX_new(ssl_method)) == NULL) {
		errError("17.12.2011 03:44:47 SSL_CTX_new() error");
		return false;
	}

	if(SSL_CTX_use_certificate_file(ssl_ctx,sslCertificateFile,SSL_FILETYPE_PEM) != 1) {
		errError("17.12.2011 03:44:56 SSL_CTX_use_certificate_file() error");
		return false;
	}

	if(SSL_CTX_use_PrivateKey_file(ssl_ctx,sslPrivateKeyFile,SSL_FILETYPE_PEM) != 1) {
		errError("17.12.2011 03:45:02 SSL_CTX_use_PrivateKey_file() error");
		return false;
	}

	if ((ssl = SSL_new(ssl_ctx)) == NULL ) {
		errError("17.12.2011 03:45:09 SSL_new() error");
		return false;
	}

	if(!SSL_CTX_check_private_key(ssl_ctx)) {
		errError("17.12.2011 03:45:19 SSL_CTX_check_private_key() error");
		return false;
	}

	if(SSL_set_fd(ssl, connectionSocket) != 1) {
		errError("17.12.2011 03:45:25 SSL_set_fd() error");
		return false;
	}

	if ((ret = SSL_accept(ssl)) <= 0) {
		int savedErrno = errno;
		errError("17.12.2011 03:45:30 SSL_accept() error");
		sslError(ret,savedErrno);
		return false;
	}
	return true;
}

bool MouseClientHandler::sendReady() {
	unsigned char *buffer = new unsigned char[1];
	unsigned char a = MSG_READY;
	memcpy(buffer,&a,sizeof(char));
	if ( SSL_write(ssl, buffer, sizeof(char)) != sizeof(char) ) {
		logger->error("11.12.2011 23:22:32 sendReady(): SSL_write() could not send MSG_READY");
		delete[] buffer;
		return false;
	}
	delete[] buffer;
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

void MouseClientHandler::errError(const char* msg) {
	unsigned long e;
	char errBuffer[128];
	logger->error(msg);
	while ( (e = ERR_get_error()) != 0 ) {
		sprintf(errBuffer,"17.12.2011 03:44:31 %lu: %s",e,ERR_error_string(e,NULL));
		logger->error(errBuffer);
	}
}

void MouseClientHandler::sslError(int ret, int savedErrno) {
	unsigned long e;
	char errBuffer[256];
	e = SSL_get_error(ssl,ret);
	sprintf(errBuffer,"%d;%lu: %s",ret,e,ERR_error_string(e,NULL) );
	if ((e == SSL_ERROR_SYSCALL) && (errno != 0)) {
		sprintf(errBuffer,"%s, errno: (%d) %s",errBuffer,savedErrno, strerror(savedErrno) );
	}
	logger->error(errBuffer);
}

bool MouseClientHandler::readyMouseHandler(){

	if (!(mouseHandler = new MouseHandler(mouseFilePath,logger)) ) {
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

