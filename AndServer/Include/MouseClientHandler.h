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

#ifndef MOUSE_CLIENT_HANDLER_H_INCLUDED
#define MOUSE_CLIENT_HANDLER_H_INCLUDED

#include <poll.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/tcp.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "messageTypes.h"
#include "MouseHandler.h"

class MouseClientHandler {
	public:
		MouseClientHandler(const int connectionSocketArg, class Logger *loggerArg, char* mouseSemName,
					char* sslCertificateFileArg, char* sslPrivateKeyFileArg, char* mouseFilePath);
		~MouseClientHandler();
		bool handleClient();

	private:
		bool readyMouseHandler();
		bool sendReady();
		bool receiveMouseMove(const int msgLength, const char* message);
		bool receiveMouseVScroll(const int msgLength, const char* message);
		bool receiveMouseHScroll(const int msgLength, const char* message);
		bool sslInit();
		void errError(const char* msg);
		void sslError(int ret, int savedErrno);

		bool receivedEndSignal;
		bool receivedEndMessage;
		int connectionSocket;

		char *sslCertificateFile;
		char *sslPrivateKeyFile;
		const SSL_METHOD *ssl_method;
		SSL_CTX *ssl_ctx;
		SSL *ssl;

		sem_t *mouseSem;

		class Logger *logger;

		char* mouseFilePath;
		class MouseHandler *mouseHandler;
};

#endif

