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

// Message types

#define KBD_TEXT 0
#define KBD_SPECIAL 1
#define MOUSE_MOVE 2
#define MOUSE_LEFT_BTN_PRESS 3
#define MOUSE_LEFT_BTN_RELEASE 4
#define MOUSE_RIGHT_BTN_PRESS 5
#define MOUSE_RIGHT_BTN_RELEASE 6
#define MOUSE_MIDDLE_BTN_PRESS 7
#define MOUSE_MIDDLE_BTN_RELEASE 8
#define SCROLL_VERT 9
#define SCROLL_HORIZ 10
#define MSG_READY 11
#define MSG_ERR 12
#define MSG_POLL 13

// Special keys

#define KEY_LCTRL 0
#define KEY_RCTRL 1
#define KEY_LALT 2
#define KEY_RALT 3
#define KEY_LSHIFT 4
#define KEY_RSHIFT 5
#define KEY_F1 6
#define KEY_F2 7
#define KEY_F3 8
#define KEY_F4 9
#define KEY_F5 10
#define KEY_F6 11
#define KEY_F7 12
#define KEY_F8 13
#define KEY_F9 14
#define KEY_F10 15
#define KEY_F11 16
#define KEY_F12 17
#define KEY_ESC 18
#define KEY_BACKSPACE 19
#define KEY_PAGEUP 20
#define KEY_PAGEDOWN 21
#define KEY_ENTER 22
#define KEY_TAB 23
#define KEY_UP 24
#define KEY_DOWN 25
#define KEY_LEFT 26
#define KEY_RIGHT 27
#define KEY_PRINTSCRN 28
#define KEY_SYSRQ KEY_PRINTSCRN
#define KEY_KP1 29
#define KEY_KP2 30
#define KEY_KP3 31
#define KEY_KP4 32
#define KEY_KP5 33
#define KEY_KP6 34
#define KEY_KP7 35
#define KEY_KP8 36
#define KEY_KP9 37
#define KEY_KP0 38
#define KEY_VOLUMEUP 39
#define KEY_VOLUMEDOWN 40
#define KEY_MUTE 41
#define KEY_PLAYPAUSE 42
#define KEY_PREVIOUSSONG 43
#define KEY_NEXTSONG 44
#define KEY_HOME 45
#define KEY_END 46
#define KEY_META 47
#define KEY_DELETE 48
