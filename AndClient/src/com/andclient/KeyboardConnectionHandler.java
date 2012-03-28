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

import java.io.IOException;
import java.io.OutputStream;

import android.util.Log;

public class KeyboardConnectionHandler extends ConnectionHandler {

	public KeyboardConnectionHandler(String host, int port) throws Exception {
		super(host, port);
	}

	public boolean sendText(String text) {
		try {
			OutputStream os = msslSocket.getOutputStream();
			byte[] buffer = new byte[5+text.length()];
			buffer[0] = MessageTypes.KBD_TEXT;
			System.arraycopy(intToByteArray(text.length()), 0, buffer, 1, 4);
			for (int i=0; i<text.length(); ++i) {
				buffer[i+5] = (byte)text.charAt(i);
			}
			synchronized(this) {
				os.write(buffer);
			}
		} catch (IOException e) {
			Log.e("KeyboardConnectionHandler","2011-12-21 13:57:13 " + e.toString());
			return false;
		}
		return true;
	}
	
	public boolean sendSpecialKeys(byte[] text) {
		try {
			OutputStream os = msslSocket.getOutputStream();
			byte[] buffer = new byte[5+text.length];
			buffer[0] = MessageTypes.KBD_SPECIAL;
			System.arraycopy(intToByteArray(text.length), 0, buffer, 1, 4);
			for (int i=0; i<text.length; ++i) {
				buffer[i+5] = text[i];
			}
			synchronized(this) {
				os.write(buffer);
			}
		} catch (IOException e) {
			Log.e("KeyboardConnectionHandler", "2012-03-02 23:44:26 " + e.toString());
			return false;
		}
		return true;
	}
	
	private byte[] intToByteArray(int value) {
		return new byte[] {
				(byte)(value >>> 24),
				(byte)(value >>> 16),
				(byte)(value >>> 8),
				(byte)value
		};
	}
}
