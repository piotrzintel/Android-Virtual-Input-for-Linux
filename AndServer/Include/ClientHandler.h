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

#ifndef CLIENT_HANDLER_H_INCLUDED
#define CLIENT_HANDLER_H_INCLUDED

#include <poll.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/tcp.h>

#include "messageTypes.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"

class ClientHandler {
	public:
		ClientHandler(const int connectionSocketArg, class Logger *loggerArg, char* semName, char* sslCertificateFileArg, 
					char* sslPrivateKeyFileArg, char* sslClientCertificateFileArg, char* filePathArg, bool verifyPeerCertificateArg);
		~ClientHandler();
		virtual bool handleClient() = 0;

	protected:
		virtual bool readyDeviceHandler() = 0;
		bool sendReady();
		bool sslInit();
		void errError(const char* msg);
		void sslError(int ret, int savedErrno);

		bool receivedEndSignal;
		bool receivedEndMessage;
		int connectionSocket;

		char *sslCertificateFile;
		char *sslPrivateKeyFile;
		char *sslClientCertificateFile;
#if (OPENSSL_VERSION_NUMBER >  0x0009080df ) // 0.9.8n
		const SSL_METHOD *ssl_method;
#else 
		SSL_METHOD *ssl_method;
#endif
		SSL_CTX *ssl_ctx;
		SSL *ssl;
		bool verifyPeerCertificate;

		sem_t *semaphore;

		class Logger *logger;

		char *deviceFilePath;
};

#endif

