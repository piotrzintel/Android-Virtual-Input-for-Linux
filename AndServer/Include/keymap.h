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

//Event types definitions:
#define AVKBD_KEY_GRAVE 0
#define AVKBD_KEY_ESC 1
#define AVKBD_KEY_1 2
#define AVKBD_KEY_2 3
#define AVKBD_KEY_3 4
#define AVKBD_KEY_4 5
#define AVKBD_KEY_5 6
#define AVKBD_KEY_6 7
#define AVKBD_KEY_7 8
#define AVKBD_KEY_8 9
#define AVKBD_KEY_9 10
#define AVKBD_KEY_0 11
#define AVKBD_KEY_MINUS 12
#define AVKBD_KEY_EQUAL 13
#define AVKBD_KEY_BACKSPACE 14
#define AVKBD_KEY_TAB 15
#define AVKBD_KEY_Q 16
#define AVKBD_KEY_W 17
#define AVKBD_KEY_E 18
#define AVKBD_KEY_R 19
#define AVKBD_KEY_T 20
#define AVKBD_KEY_Y 21
#define AVKBD_KEY_U 22
#define AVKBD_KEY_I 23
#define AVKBD_KEY_O 24
#define AVKBD_KEY_P 25
#define AVKBD_KEY_LEFTBRACE 26
#define AVKBD_KEY_RIGHTBRACE 27
#define AVKBD_KEY_BACKSLASH 28
#define AVKBD_KEY_CAPSLOCK 29
#define AVKBD_KEY_A 30
#define AVKBD_KEY_S 31
#define AVKBD_KEY_D 32
#define AVKBD_KEY_F 33
#define AVKBD_KEY_G 34
#define AVKBD_KEY_H 35
#define AVKBD_KEY_J 36
#define AVKBD_KEY_K 37
#define AVKBD_KEY_L 38
#define AVKBD_KEY_SEMICOLON 39
#define AVKBD_KEY_APOSTROPHE 40
#define AVKBD_KEY_ENTER 41
#define AVKBD_KEY_LEFTSHIFT 42
#define AVKBD_KEY_Z 43
#define AVKBD_KEY_X 44
#define AVKBD_KEY_C 45
#define AVKBD_KEY_V 46
#define AVKBD_KEY_B 47
#define AVKBD_KEY_N 48
#define AVKBD_KEY_M 49
#define AVKBD_KEY_COMMA 50
#define AVKBD_KEY_DOT 51
#define AVKBD_KEY_SLASH 52
#define AVKBD_KEY_RIGHTSHIFT 53
#define AVKBD_KEY_LEFTCTRL 54
#define AVKBD_KEY_LEFTMETA 55
#define AVKBD_KEY_LEFTALT 56
#define AVKBD_KEY_SPACE 57
#define AVKBD_KEY_RIGHTALT 58
#define AVKBD_KEY_RIGHTCTRL 59
#define AVKBD_KEY_UP 60
#define AVKBD_KEY_DOWN 61
#define AVKBD_KEY_LEFT 62
#define AVKBD_KEY_RIGHT 63
#define AVKBD_KEY_PAGEUP 64
#define AVKBD_KEY_PAGEDOWN 65
#define AVKBD_KEY_F1 66
#define AVKBD_KEY_F2 67
#define AVKBD_KEY_F3 68
#define AVKBD_KEY_F4 69
#define AVKBD_KEY_F5 70
#define AVKBD_KEY_F6 71
#define AVKBD_KEY_F7 72
#define AVKBD_KEY_F8 73
#define AVKBD_KEY_F9 74
#define AVKBD_KEY_F10 75
#define AVKBD_KEY_F11 76
#define AVKBD_KEY_F12 77
#define AVKBD_KEY_HOME 78
#define AVKBD_KEY_END 79
#define AVKBD_KEY_INSERT 80
#define AVKBD_KEY_DELETE 81
#define AVKBD_KEY_SYSRQ 82
#define AVKBD_KEY_PRINTSCRN AVKBD_KEY_SYSRQ
#define AVKBD_KEY_KP1 83
#define AVKBD_KEY_KP2 84
#define AVKBD_KEY_KP3 85
#define AVKBD_KEY_KP4 86
#define AVKBD_KEY_KP5 87
#define AVKBD_KEY_KP6 88
#define AVKBD_KEY_KP7 89
#define AVKBD_KEY_KP8 90
#define AVKBD_KEY_KP9 91
#define AVKBD_KEY_KP0 92
#define AVKBD_KEY_VOLUMEUP 93
#define AVKBD_KEY_VOLUMEDOWN 94
#define AVKBD_KEY_MUTE 95
#define AVKBD_KEY_PLAYPAUSE 96
#define AVKBD_KEY_PREVIOUSSONG 97
#define AVKBD_KEY_NEXTSONG 98

#define AVKBD_KEYMAP_SIZE 0x63

