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

#include "ClientHandler.h"

ClientHandler::ClientHandler(const int connectionSocketArg, class Logger *loggerArg, char* semName, char* sslCertificateFileArg, 
					char* sslPrivateKeyFileArg, char* sslClientCertificateFileArg, char* filePathArg, bool verifyPeerCertificateArg) {
	sslCertificateFile = strcpy(new char[strlen(sslCertificateFileArg) + 1],sslCertificateFileArg);
	sslPrivateKeyFile = strcpy(new char[strlen(sslPrivateKeyFileArg) + 1],sslPrivateKeyFileArg);
	sslClientCertificateFile = strcpy(new char[strlen(sslClientCertificateFileArg) + 1],sslClientCertificateFileArg);
	deviceFilePath = strcpy(new char[strlen(filePathArg) + 1],filePathArg);
	connectionSocket = connectionSocketArg;
	receivedEndSignal = false;
	receivedEndMessage = false;
	logger = loggerArg;
	ssl_method = NULL;
	ssl_ctx = NULL;
	ssl = NULL;
	verifyPeerCertificate = verifyPeerCertificateArg;
	if ((semaphore = sem_open(semName,0,O_RDWR,0)) == SEM_FAILED) {
		logger->error("sem_open failed",errno);
	}
}

ClientHandler::~ClientHandler() {
	sem_close(semaphore);
	if (ssl) SSL_shutdown(ssl);
	if (ssl) SSL_free(ssl);
	if (ssl_ctx) SSL_CTX_free(ssl_ctx);
	if (sslCertificateFile) delete[] sslCertificateFile;
	if (sslPrivateKeyFile) delete[] sslPrivateKeyFile;
	if (sslClientCertificateFile) delete[] sslClientCertificateFile;
	if (deviceFilePath) delete[] deviceFilePath;
	close(connectionSocket);
}

bool ClientHandler::sslInit() {
	int ret = 0;
	OpenSSL_add_all_algorithms();
	SSL_library_init() ;
	SSL_load_error_strings();

	if ( (ssl_method = TLSv1_server_method()) == NULL ) {
		errError("_server_method() error");
		return false;
	}

	if((ssl_ctx = SSL_CTX_new(ssl_method)) == NULL) {
		errError("16.12.2011 22:15:09 SSL_CTX_new() error");
		return false;
	}

	if(SSL_CTX_use_certificate_file(ssl_ctx, sslCertificateFile, SSL_FILETYPE_PEM) != 1) {
		errError("17.12.2011 00:12:29 SSL_CTX_use_certificate_file() error");
		return false;
	}

	if(SSL_CTX_use_PrivateKey_file(ssl_ctx, sslPrivateKeyFile, SSL_FILETYPE_PEM) != 1) {
		errError("17.12.2011 00:12:33 SSL_CTX_use_PrivateKey_file() error");
		return false;
	}

	// ==== BEGIN CLIENT CERT VERIFICATION
	if (verifyPeerCertificate) {

	STACK_OF(X509_NAME) *list = NULL;

		if (SSL_CTX_load_verify_locations(ssl_ctx, sslClientCertificateFile, NULL) != 1) {
			errError("22.10.2012 23:37:46 SSL_CTX_load_verify_locations() error");
			return false;
		}

		if ((list = SSL_load_client_CA_file( sslClientCertificateFile )) == NULL ) {
			errError("23.10.2012 16:00:51 SSL_load_client_CA_file() error");
			return false;
		}

		SSL_CTX_set_client_CA_list(ssl_ctx, list);

		SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT , NULL);
	
	}
	// ==== END CLIENT CERT VERIFICATION

	if ((ssl = SSL_new(ssl_ctx)) == NULL ) {
		errError("16.12.2011 22:15:15 SSL_new() error");
		return false;
	}

	if(!SSL_CTX_check_private_key(ssl_ctx)) {
		errError("16.12.2011 22:15:22 SSL_CTX_check_private_key() error");
		return false;
	}

	if(SSL_set_fd(ssl, connectionSocket) != 1) {
		errError("17.12.2011 01:57:52 SSL_set_fd() error");
		return false;
	}

	if ((ret = SSL_accept(ssl)) <= 0) {
		int savedErrno = errno;
		errError("16.12.2011 22:15:33 SSL_accept() error");
		sslError(ret,savedErrno);
		return false;
	}
	return true;
}

bool ClientHandler::sendReady() {
	unsigned char *buffer = new unsigned char[1];
	unsigned char a = MSG_READY;
	memcpy(buffer,&a,sizeof(char));
	if ( SSL_write(ssl, buffer, sizeof(char)) != sizeof(char) ) {
		errError("16.12.2011 22:16:40 write() error");
		delete[] buffer;
		return false;
	}
	delete[] buffer;
	return true;
}

void ClientHandler::errError(const char* msg) {
	unsigned long e;
	char errBuffer[128];
	logger->error(msg);
	while ( (e = ERR_get_error()) != 0 ) {
		sprintf(errBuffer,"16.12.2011 00:33:38 %lu: %s",e,ERR_error_string(e,NULL));
		logger->error(errBuffer);
	}
}

void ClientHandler::sslError(int ret, int savedErrno) {
	unsigned long e;
	char errBuffer[256];
	e = SSL_get_error(ssl,ret);
	sprintf(errBuffer,"%d;%lu: %s",ret,e,ERR_error_string(e,NULL) );
	if ((e == SSL_ERROR_SYSCALL) && (errno != 0)) {
		sprintf(errBuffer,"%s, errno: (%d) %s",errBuffer,savedErrno, strerror(savedErrno) );
	}
	logger->error(errBuffer);
}

