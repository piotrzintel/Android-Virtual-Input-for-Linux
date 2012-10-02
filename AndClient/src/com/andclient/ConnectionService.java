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

import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Process;
import android.util.Log;

public class ConnectionService extends Service {

	public static ConnectionService mService = null;
	public static final String CONNECTION_LOST_INTENT = "com.avinputdevice.andclient.intent.CONNECTION_LOST";
	public static final String DISCONNECTED_INTENT = "com.avinputdevice.andclient.intent.DISCONNECTED";
	public static final String CONNECTED_INTENT = "com.avinputdevice.andclient.intent.CONNECTED";
	public static final String CONNECTION_FAILED_INTENT = "com.avinputdevice.andclient.intent.CONNECTION_FAILED";
	public static final String CONNECTING_INTENT = "com.avinputdevice.andclient.intent.CONNECTING";
		
	private MouseConnectionHandler mMouseConnectionHandler = null;
	private KeyboardConnectionHandler mKeyboardConnectionHandler = null;
	private SharedPreferences settings;
	
	private static Looper mMouseServiceLooper;
	private static Looper mKeyboardServiceLooper;
	
	private static ServiceHandler mMouseServiceHandler;
	private static ServiceHandler mKeyboardServiceHandler;
	
	private final class ServiceHandler extends Handler {
		public ServiceHandler(Looper looper) {
			super(looper);
		}
	}

	private static boolean mIsMouseConnected = false;
	private static boolean mIsKeyboardConnected = false;
	
	private static final Object mSendBroadcastLock = new Object();
	private static final Object mMouseConnectionSendLock = new Object();
	private static final Object mKeyboardConnectionSendLock = new Object();
	
	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}

	@Override
	public void onCreate() {
		
		sendConnectionBroadcast(ConnectionService.CONNECTING_INTENT);
		
		new Thread(new Runnable() {
			
			@Override
			public void run() {
		
				// MOUSE THREAD
				HandlerThread thread = new HandlerThread("MouseConnectionServiceHandlerThread", Process.THREAD_PRIORITY_BACKGROUND);
				thread.start();
				mMouseServiceLooper = thread.getLooper();
				mMouseServiceHandler = new ServiceHandler(mMouseServiceLooper);
				// END MOUSE THREAD
				
				// KEYBOARD THREAD
				thread = new HandlerThread("KeyboardConnectionServiceHandlerThread", Process.THREAD_PRIORITY_BACKGROUND);
				thread.start();
				mKeyboardServiceLooper = thread.getLooper();
				mKeyboardServiceHandler = new ServiceHandler(mKeyboardServiceLooper);
				// END KEYBOARD THREAD
						
		
				// READ SETTINGS
				settings = getSharedPreferences(StartActivity.PREFERENCES_FILE, 0);
				int mousePort = settings.getInt(StartActivity.PREF_MOUSE_PORT, -1);
				int keyboardPort = settings.getInt(StartActivity.PREF_KEYBOARD_PORT, -1);
				String serverIp = settings.getString(StartActivity.PREF_SERVER_IP, null);
				// END READ SETTINGS
				
				// SETUP CONNECTION HANDLERS WITH THREADS
				if ( mousePort != -1 && keyboardPort != -1 && serverIp != null )
				{
					try {
						mMouseConnectionHandler = new MouseConnectionHandler(serverIp, mousePort);
					} catch (Exception e) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_FAILED_INTENT, e.getMessage());
						stopSelf();
					}
					if (mMouseConnectionHandler != null)
					{
						mMouseServiceHandler.post(new Runnable() {
							public void run() {
								try {
									if (mMouseConnectionHandler.Connect()) {
										mIsMouseConnected = true;
										hasConnectedSendBroadcast();
									}
								} catch (Exception e) {
									sendConnectionBroadcast(ConnectionService.CONNECTION_FAILED_INTENT, e.getMessage());
									stopSelf();
								}
							}
						});
					}
					
					try {
						mKeyboardConnectionHandler = new KeyboardConnectionHandler(serverIp, keyboardPort);
					} catch (Exception e) {
							sendConnectionBroadcast(ConnectionService.CONNECTION_FAILED_INTENT, e.getMessage());
							stopSelf();
					}
					if (mKeyboardConnectionHandler != null)
					{
						mKeyboardServiceHandler.post(new Runnable() {
							public void run() {
								try {
									if (mKeyboardConnectionHandler.Connect()) {
										mIsKeyboardConnected = true;
										hasConnectedSendBroadcast();
									}
									new Thread(new Runnable() {
										public void run() {
											pollConnection();
										}
									}).start();
								} catch (Exception e) {
									sendConnectionBroadcast(ConnectionService.CONNECTION_FAILED_INTENT, e.getMessage());
									stopSelf();
								}
							}
						});
					}
				}
				else {
					stopSelf();
				}
				// END SETUP CONNECTION HANDLERS WITH THREADS
			}
		}).run();
	}
	
	@Override
	public void onDestroy()	{
		if (mMouseConnectionHandler != null) {
			mMouseConnectionHandler.close();	
		}
		if (mKeyboardConnectionHandler != null) {
			mKeyboardConnectionHandler.close();	
		}
		
		mService = null;
		sendConnectionBroadcast(ConnectionService.DISCONNECTED_INTENT);
	}
	
	public void sendMouseMove(final int Xvalue, final int Yvalue) {
		mMouseServiceHandler.post(new Runnable() {
			@Override
			public void run() {
				synchronized (mMouseConnectionSendLock) {
					try {
						if (!mMouseConnectionHandler.SendMouseMove(Xvalue, Yvalue)) {
							sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
							stopSelf();
						}
					} catch (NullPointerException ex) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			}
		});
	}
	
	public void sendMouseScroll(final byte axis, final int Xvalue) {
		mMouseServiceHandler.post(new Runnable() {
			@Override
			public void run() {
				synchronized (mMouseConnectionSendLock) {
					try {
						if (!mMouseConnectionHandler.SendMouseScroll(axis, Xvalue)) {
							sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
							stopSelf();
						}
					} catch (NullPointerException ex) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			}
		});
	}
	
	public void sendMouseButtonPress(final byte button) {
		mMouseServiceHandler.post(new Runnable() {
			@Override
			public void run() {
				synchronized (mMouseConnectionSendLock) {
					try {
						if(!mMouseConnectionHandler.SendMouseButtonPress(button)) {
							sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
							stopSelf();
						}
					} catch (NullPointerException ex) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			}
		});
	}
	
	public void sendMouseButtonRelease(final byte button) {
		mMouseServiceHandler.post(new Runnable() {
			@Override
			public void run() {
				synchronized (mMouseConnectionSendLock) {
					try {
						if(!mMouseConnectionHandler.SendMouseButtonRelease(button)) {
							sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
							stopSelf();
						}
					} catch (NullPointerException ex) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			}
		});
	}
	
	public void sendKeyboardText(final String text) {
		mKeyboardServiceHandler.post(new Runnable() {
			@Override
			public void run() {
				synchronized (mKeyboardConnectionSendLock) {
					try {
						if(!mKeyboardConnectionHandler.sendText(text)) {
							sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
							stopSelf();
						}
					} catch (NullPointerException ex) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			}
		});
	}
	
	public void sendKeyboardSpecial(final byte[] text) {
		mKeyboardServiceHandler.post(new Runnable() {
			@Override
			public void run() {
				synchronized (mKeyboardConnectionSendLock) {
					try {
						if(!mKeyboardConnectionHandler.sendSpecialKeys(text)) {
							sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
							stopSelf();
						}
					} catch (NullPointerException ex) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			}
		});
	}
	
	private void pollConnection() {
		while (mService != null) {
			mMouseServiceHandler.post(new Runnable() {
				@Override
				public void run() {
					try {
						mMouseConnectionHandler.pollConnectionStatus();
					} catch (Exception e) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			});
			
			mKeyboardServiceHandler.post(new Runnable() {
				@Override
				public void run() {
					try {
						mKeyboardConnectionHandler.pollConnectionStatus();
					} catch (Exception e) {
						sendConnectionBroadcast(ConnectionService.CONNECTION_LOST_INTENT);
						stopSelf();
					}
				}
			});
			
			try {
				Thread.sleep(5000);
			} catch (InterruptedException e) {
				Log.e("ConnectionService", "2012-03-02 15:45:58 " + e.toString());
			}
		}
	}
	
	private void sendConnectionBroadcast(String action) {
		synchronized(mSendBroadcastLock) {
			Intent intent = new Intent();
			intent.setAction(action);
			this.sendBroadcast(intent);
		}
	}
	
	private void sendConnectionBroadcast(String action, String error) {
		synchronized(mSendBroadcastLock) {
			Intent intent = new Intent();
			intent.setAction(action);
			intent.putExtra("ErrorText", error);
			this.sendBroadcast(intent);
		}
	}
	
	private void hasConnectedSendBroadcast() {
		synchronized(mSendBroadcastLock) {
			if (mIsKeyboardConnected && mIsMouseConnected && mService == null) {
				mService = ConnectionService.this;
				sendConnectionBroadcast(ConnectionService.CONNECTED_INTENT);
			}
		}
	}
}
