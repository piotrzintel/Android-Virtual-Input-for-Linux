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

#include "ClientHandler.h"
#include <sys/time.h> //setpriority()
#include <sys/resource.h> //setpriority()

class MouseClientHandler : ClientHandler {
	public:
		MouseClientHandler(const int connectionSocketArg, class Logger *loggerArg, char* mouseSemName,
					char* sslCertificateFileArg, char* sslPrivateKeyFileArg, char* mouseFilePath);
		~MouseClientHandler();
		bool handleClient();

	private:
		bool readyDeviceHandler();
		bool receiveMouseMove(const int msgLength, const char* message);
		bool receiveMouseVScroll(const int msgLength, const char* message);
		bool receiveMouseHScroll(const int msgLength, const char* message);

		class MouseHandler *mouseHandler;
};

#endif

