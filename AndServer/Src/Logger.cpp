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

#include "Logger.h"

Logger::Logger() {
	isDaemon = true;
}

void Logger::printMessage(const char *message) {
	if ( isDaemon )
		syslog(LOG_INFO, "%s", message);
	else
		cout << message << endl;
}

void Logger::error(const char *message) {
	if ( isDaemon )
		syslog(LOG_ERR, "%s", message);
	else
		cerr << message << endl;
}

void Logger::error(const char *message, const int localerrno) {
	char buffer[256];
	sprintf(buffer, "%s: (%d) %s",message,localerrno,strerror(localerrno));
	error(buffer);
}
