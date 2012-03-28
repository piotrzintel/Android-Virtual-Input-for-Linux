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

public class MouseConnectionHandler extends ConnectionHandler {
	
	public MouseConnectionHandler(String host, int port) throws Exception {
		super(host, port);
	}
	public boolean SendMouseMove(final int Xvalue, final int Yvalue) {
		OutputStream os = null;
		try {
			synchronized(this)
			{
				os = msslSocket.getOutputStream();
			}
			byte buffer[] = new byte[13];
			buffer[0] = MessageTypes.MOUSE_MOVE;
			System.arraycopy(intToByteArray(8), 0, buffer, 1, 4);
			System.arraycopy(intToByteArray(Xvalue), 0, buffer, 5, 4);
			System.arraycopy(intToByteArray(Yvalue), 0, buffer, 9, 4);
			synchronized(this)
			{
				os.write(buffer);
			}
			return true;
		} catch (IOException e) {
			Log.e("MouseConnectionHandler", "2012-03-02 23:02:49 " + e.toString());
			return false;
		}
	}
	
	public boolean SendMouseButtonPress(final byte button) {
		synchronized(this)
		{
			try {
				OutputStream os = msslSocket.getOutputStream();
				byte buffer[] = new byte[5];
				switch (button) {
				
				case 0:
					buffer[0] = MessageTypes.MOUSE_LEFT_BTN_PRESS;
					for (int i=1;i<5;++i) {
						buffer[i] = 0;
					}
					os.write(buffer);
				break;
				
				case 1:
					buffer[0] = MessageTypes.MOUSE_RIGHT_BTN_PRESS;
					for (int i=1;i<5;++i) {
						buffer[i] = 0;
					}
					os.write(buffer);
				break;
				
				case 2:
					buffer[0] = MessageTypes.MOUSE_MIDDLE_BTN_PRESS;
					for (int i=1;i<5;++i) {
						buffer[i] = 0;
					}
					os.write(buffer);
				break;
				}
				
			} catch (IOException e) {
				Log.e("MouseConnectionHandler", "2012-03-02 23:02:57 " + e.toString());
				return false;
			}
			return true;
		}
	}
	
	public boolean SendMouseButtonRelease(final byte button) {
		synchronized(this)
		{
			try {
				OutputStream os = msslSocket.getOutputStream();
				byte buffer[] = new byte[5];
				switch (button) {
				
				case 0:
					buffer[0] = MessageTypes.MOUSE_LEFT_BTN_RELEASE;
					for (int i=1;i<5;++i) {
						buffer[i] = 0;
					}
					os.write(buffer);
				break;
				
				case 1:
					buffer[0] = MessageTypes.MOUSE_RIGHT_BTN_RELEASE;
					for (int i=1;i<5;++i) {
						buffer[i] = 0;
					}
					os.write(buffer);
				break;
				
				case 2:
					buffer[0] = MessageTypes.MOUSE_MIDDLE_BTN_RELEASE;
					for (int i=1;i<5;++i) {
						buffer[i] = 0;
					}
					os.write(buffer);
				break;
				}
				return true;
			} catch (IOException e) {
				Log.e("MouseConnectionHandler", "2012-03-02 23:03:05 " + e.toString());
				return false;
			}
		}
	}
	
	public boolean SendMouseScroll(final byte axis, final int Xvalue) {
		OutputStream os = null;
		try {
			synchronized(this)
			{
				os = msslSocket.getOutputStream();
			}
			byte buffer[] = new byte[9];
			buffer[0] = axis;
			System.arraycopy(intToByteArray(4), 0, buffer, 1, 4);
			System.arraycopy(intToByteArray(Xvalue), 0, buffer, 5, 4);
			synchronized(this)
			{
				os.write(buffer);
			}
			return true;
		} catch (IOException e) {
			Log.e("MouseConnectionHandler", "2012-03-02 23:02:49 " + e.toString());
			return false;
		}
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
