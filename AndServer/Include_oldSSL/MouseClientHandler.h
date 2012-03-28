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
		SSL_METHOD *ssl_method;
		SSL_CTX *ssl_ctx;
		SSL *ssl;

		sem_t *mouseSem;

		class Logger *logger;

		char* mouseFilePath;
		class MouseHandler *mouseHandler;
};

#endif

