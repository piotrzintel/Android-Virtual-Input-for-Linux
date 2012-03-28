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

#include "Main.h"

bool AndroidInputServer::receivedEndSignal = false;
bool AndroidInputServer::receivedSigChild = false;
int AndroidInputServer::activeConnections = 0;

struct sigaction sigactionEnd;
struct sigaction sigactionChild;

int main(int argc, char *argv[]) {

	AndroidInputServer *server = new AndroidInputServer();
	if (server != NULL) {
		if ( !(server->initialize(argc, argv)) ) {
			delete server;
			return 1;
		}
	} else {
		return 1;
	}

	sigactionEnd.sa_handler = server->handleEndSignal;
	sigactionChild.sa_handler = server->handleSigChild;

	sigaction(SIGINT, &sigactionEnd, NULL);
	sigaction(SIGTERM, &sigactionEnd, NULL);
	sigaction(SIGCHLD, &sigactionChild, NULL);

	server->andListen();

	delete server;
	return 0;
}

