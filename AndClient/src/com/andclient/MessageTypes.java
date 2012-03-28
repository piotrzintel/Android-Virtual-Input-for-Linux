/*

   AndClient, part of Android™ Virtual Input for LInux project

   Copyright 2012 Piotr Zintel
   zintelpiotr@gmail.com

   Android is a trademark of Google Inc.

*/

/*

   AndClient is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   AndClient is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

package com.andclient;

public class MessageTypes {
	
	// Message types
	
	public static final byte KBD_TEXT = 0;
	public static final byte KBD_SPECIAL = 1;
	public static final byte MOUSE_MOVE = 2;
	public static final byte MOUSE_LEFT_BTN_PRESS = 3;
	public static final byte MOUSE_LEFT_BTN_RELEASE = 4;
	public static final byte MOUSE_RIGHT_BTN_PRESS = 5;
	public static final byte MOUSE_RIGHT_BTN_RELEASE = 6;
	public static final byte MOUSE_MIDDLE_BTN_PRESS = 7;
	public static final byte MOUSE_MIDDLE_BTN_RELEASE = 8;
	public static final byte SCROLL_VERT = 9;
	public static final byte SCROLL_HORIZ = 10;
	public static final byte MSG_READY = 11;
	public static final byte MSG_ERROR = 12;
	public static final byte MSG_POLL = 13;

	// Special keys
	
	 public static final byte KEY_LCTRL = 0;
	 public static final byte KEY_RCTRL = 1;
	 public static final byte KEY_LALT = 2;
	 public static final byte KEY_RALT = 3;
	 public static final byte KEY_LSHIFT = 4;
	 public static final byte KEY_RSHIFT = 5;
	 public static final byte KEY_F1 = 6;
	 public static final byte KEY_F2 = 7;
	 public static final byte KEY_F3 = 8;
	 public static final byte KEY_F4 = 9;
	 public static final byte KEY_F5 = 10;
	 public static final byte KEY_F6 = 11;
	 public static final byte KEY_F7 = 12;
	 public static final byte KEY_F8 = 13;
	 public static final byte KEY_F9 = 14;
	 public static final byte KEY_F10 = 15;
	 public static final byte KEY_F11 = 16;
	 public static final byte KEY_F12 = 17;
	 public static final byte KEY_ESC = 18;
	 public static final byte KEY_BACKSPACE = 19;
	 public static final byte KEY_PAGEUP = 20;
	 public static final byte KEY_PAGEDOWN = 21;
	 public static final byte KEY_ENTER = 22;
	 public static final byte KEY_TAB = 23;
	 public static final byte KEY_UP = 24;
	 public static final byte KEY_DOWN = 25;
	 public static final byte KEY_LEFT = 26;
	 public static final byte KEY_RIGHT = 27;
	 public static final byte KEY_PRINTSCRN = 28;
	 public static final byte KEY_SYSRQ = KEY_PRINTSCRN;
	 public static final byte KEY_KP1 = 29;
	 public static final byte KEY_KP2 = 30;
	 public static final byte KEY_KP3 = 31;
	 public static final byte KEY_KP4 = 32;
	 public static final byte KEY_KP5 = 33;
	 public static final byte KEY_KP6 = 34;
	 public static final byte KEY_KP7 = 35;
	 public static final byte KEY_KP8 = 36;
	 public static final byte KEY_KP9 = 37;
	 public static final byte KEY_KP0 = 38;
	 public static final byte KEY_VOLUMEUP = 39;
	 public static final byte KEY_VOLUMEDOWN = 40;
	 public static final byte KEY_MUTE = 41;
	 public static final byte KEY_PLAYPAUSE = 42;
	 public static final byte KEY_PREVIOUSSONG = 43;
	 public static final byte KEY_NEXTSONG = 44;
	 public static final byte KEY_HOME = 45;
	 public static final byte KEY_END = 46;
	 public static final byte KEY_META = 47;
	 public static final byte KEY_DELETE = 48;
}
