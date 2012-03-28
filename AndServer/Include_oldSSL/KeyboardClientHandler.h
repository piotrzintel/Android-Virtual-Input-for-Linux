#ifndef KEYBOARD_CLIENT_HANDLER_H_INCLUDED
#define KEYBOARD_CLIENT_HANDLER_H_INCLUDED

#include <poll.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/tcp.h>

#include "messageTypes.h"
#include "KeyboardHandler.h"

class KeyboardClientHandler {
	public:
		KeyboardClientHandler(const int connectionSocketArg, class Logger *loggerArg, char* keyboardSemName,
					char* sslCertificateFileArg, char* sslPrivateKeyFileArg, char* keyboardFilePathArg);
		~KeyboardClientHandler();
		bool handleClient();

	private:
		bool readyKeyboardHandler();
		bool sendReady();
		bool receiveKbdText(const int msgLength, const char* message);
		bool receiveKbdSpecial(const int msgLength, const char* message);
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

		sem_t *keyboardSem;

		class Logger *logger;

		char *keyboardFilePath;
		class KeyboardHandler *keyboardHandler;
};

#endif

