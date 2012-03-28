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

#ifndef _ANDROID_INPUT_SERVER_H_INCLUDED
#define _ANDROID_INPUT_SERVER_H_INCLUDED

#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <libgen.h>
#include <fcntl.h>

#include "Logger.h"
#include "MouseHandler.h"
#include "KeyboardHandler.h"
#include "KeyboardClientHandler.h"
#include "MouseClientHandler.h"

#define KEYBOARD_DEFAULT_PORT 6780
#define MOUSE_DEFAULT_PORT 6781

#define KEYBOARD_DEFAULT_FILE_PATH "/dev/avkbd"
#define MOUSE_DEFAULT_FILE_PATH "/dev/avms"
#define OPTIONS_DEFAULT_FILE_PATH "/etc/andserver.conf"

class AndroidInputServer {
	public:
		AndroidInputServer();
		~AndroidInputServer();
		bool initialize(int argc, char* argv[]);
		bool andListen();
		int receiveMessage();
		int sendMessage();
		static void handleEndSignal(int s);
		static void handleSigChild(int s);

	private:
		bool isNumber(const char *string);
		void usage(const char *programName);
		bool parseOptions(int argc, char* argv[]);
		bool parseOptionsFile();
		bool getDefaultPaths();
		bool daemonize();
		bool readySockets();
		bool semaphoresInit();
		void handleClient(const int acceptedFromListeningSocket);
		int handleConnectionRequest(const int acceptedFromListeningSocket);
		int splitServer();

		static bool receivedEndSignal;
		static bool receivedSigChild;
		static int activeConnections;
		int keyboardListeningPort;
		int mouseListeningPort;
		int keyboardListeningSocket;
		int mouseListeningSocket;
		int maxConnections;
		int clientSocket;
		bool isDaemon;
		bool debug;
		char *programName;
		bool child;

		sem_t *mouseSem;
		sem_t *keyboardSem;
		char *mouseSemName;
		char *keyboardSemName;

		char *sslCertificateFile;
		char *sslPrivateKeyFile;

		struct sockaddr_in keyboardServerAddress;
		struct sockaddr_in mouseServerAddress;
		struct sockaddr_in clientAddress;

		char *optionsFilePath;
		char *keyboardFilePath;
		char *mouseFilePath;

		class KeyboardClientHandler *keyboardClientHandler;
		class MouseClientHandler *mouseClientHandler;

		class Logger *logger;
};

#endif

