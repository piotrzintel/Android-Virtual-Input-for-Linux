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

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class StartActivity extends Activity {
	public static final String PREFERENCES_FILE = "AndClientSettings";
	public static final String PREF_MOUSE_PORT = "mousePort";
	public static final String PREF_KEYBOARD_PORT = "keyboardPort";
	public static final String PREF_SERVER_IP = "serverIp";
	private MouseConnectionReceiver mMouseConnectionReceiver = new MouseConnectionReceiver();
	private ProgressDialog mProgressDialog;
	
	private class MouseConnectionReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			
			if (action.equals(ConnectionService.CONNECTING_INTENT)) {
		    	setProgressBarIndeterminateVisibility(true);  	
		    	mProgressDialog = ProgressDialog.show(StartActivity.this, "Connecting", "Pease wait for the connection to establish");
			} else {
				if (ConnectionService.mService == null) {
		    		((Button) findViewById(R.id.buttonMouseScreen)).setEnabled(false);
		    		((Button) findViewById(R.id.buttonKeyboardScreen)).setEnabled(false);
		    		((Button) findViewById(R.id.buttonConnect)).setEnabled(true);
		    		((Button) findViewById(R.id.buttonDisconnect)).setEnabled(false);
		    	}
		    	else {
		    		((Button) findViewById(R.id.buttonMouseScreen)).setEnabled(true);
		    		((Button) findViewById(R.id.buttonKeyboardScreen)).setEnabled(true);
		    		((Button) findViewById(R.id.buttonConnect)).setEnabled(false);
		    		((Button) findViewById(R.id.buttonDisconnect)).setEnabled(true);
		    	}
				
				TextView tv = (TextView) findViewById(R.id.textViewConnectionStatus);
				if (action.equals(ConnectionService.CONNECTED_INTENT)) {
					tv.setTextColor(Color.GREEN);
					tv.setText("Connected");
				} else if (action.equals(ConnectionService.DISCONNECTED_INTENT)) {
					tv.setTextColor(Color.RED);
					tv.setText("Disconnected");
				} else if (action.equals(ConnectionService.CONNECTION_FAILED_INTENT)) {
					Bundle b = intent.getExtras();
					String m = "Connection failed!";
					String s = null;
					if (b != null) {
						s = b.getString("ErrorText");
					}
					if (s != null) {
						m = m + "\n" +s;
					}
					Toast.makeText(StartActivity.this, m, 5).show();
				} else if (action.equals(ConnectionService.CONNECTION_LOST_INTENT)) {
					Toast.makeText(StartActivity.this, "Connection lost", 5);
				}
				
				if (mProgressDialog != null) {
					setProgressBarIndeterminateVisibility(false);
					mProgressDialog.dismiss();
					mProgressDialog = null;
				}
			}
		}
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.start_screen);
        
        boolean hasSettings = verifySettings();
        
        // SETUP BUTTON
        Button button = (Button) findViewById(R.id.buttonSetupScreen);
        
        button.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonSetupScreenOnClick(v);
			}
		});
        // END SETUP BUTTON
        
        // MOUSE BUTTON
        button = (Button) findViewById(R.id.buttonMouseScreen);
        button.setEnabled(hasSettings);
        
        button.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonMouseScreenOnClick(v);
			}
		});
        // END MOUSE BUTTON
        
        // KEYBOARD BUTTON
        button = (Button) findViewById(R.id.buttonKeyboardScreen);
        button.setEnabled(hasSettings);
        
        button.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonKeyboardScreenOnClick(v);
			}
		});
        // END KEYBOARD BUTTON
        
        // CONNECT BUTTON
        button = (Button) findViewById(R.id.buttonConnect);
        button.setEnabled(hasSettings);
        
        button.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonConnectOnClick(v);
			}
		});
        // END CONNECT BUTTON
        
        // DISCONNECT BUTTON
        button = (Button) findViewById(R.id.buttonDisconnect);
        button.setEnabled(hasSettings);
        
        button.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonDisconnectOnClick(v);
			}
		});
        // END DISCONNECT BUTTON
    }
    
    @Override
    public void onResume()
    {
    	super.onResume();
    	boolean hasSettings = verifySettings();
    	TextView tv = (TextView) findViewById(R.id.textViewConnectionStatus);
    	if (ConnectionService.mService == null) {
    		((Button) findViewById(R.id.buttonMouseScreen)).setEnabled(false);
    		((Button) findViewById(R.id.buttonKeyboardScreen)).setEnabled(false);
    		((Button) findViewById(R.id.buttonConnect)).setEnabled(true & hasSettings);
    		((Button) findViewById(R.id.buttonDisconnect)).setEnabled(false);
    		tv.setText("Disconnected");
    		tv.setTextColor(Color.RED);
    	}
    	else {
    		((Button) findViewById(R.id.buttonMouseScreen)).setEnabled(true);
    		((Button) findViewById(R.id.buttonKeyboardScreen)).setEnabled(true);
    		((Button) findViewById(R.id.buttonConnect)).setEnabled(false);
    		((Button) findViewById(R.id.buttonDisconnect)).setEnabled(true);
    		tv.setText("Connected");
    		tv.setTextColor(Color.GREEN);
    	}
    	IntentFilter iff = new IntentFilter();
    	iff.addAction(ConnectionService.CONNECTING_INTENT);
    	iff.addAction(ConnectionService.CONNECTED_INTENT);
    	iff.addAction(ConnectionService.DISCONNECTED_INTENT);
    	iff.addAction(ConnectionService.CONNECTION_FAILED_INTENT);
    	iff.addAction(ConnectionService.CONNECTION_LOST_INTENT);
    	registerReceiver(mMouseConnectionReceiver, iff);
    	
    	try {
	    	Bundle b = getIntent().getExtras();
	    	if (b != null) {
	    		if (b.getBoolean("ConnectionLost")) {
	    			Toast.makeText(StartActivity.this, "Connection lost!", 5).show();
	    		}
	    	}
	    	setIntent(null);
    	} catch (NullPointerException e) {}
    }
    
    @Override
    public void onPause()
    {
    	super.onPause();
    	unregisterReceiver(mMouseConnectionReceiver);
    }
    
    private void buttonSetupScreenOnClick(View v)
    {
    	Intent intent = new Intent(this, SetupConnectionActivity.class);
		startActivity(intent);
    }
    
    private void buttonMouseScreenOnClick(View v)
    {
    	Intent intent = new Intent(this, TouchpadActivity.class);
		startActivity(intent);
    }
    
    private void buttonKeyboardScreenOnClick(View v)
    {
    	Intent intent = new Intent(this, KeyboardActivity.class);
		startActivity(intent);
    }
    
    private void buttonConnectOnClick(View v)
    {
    	Intent intent = new Intent(this, ConnectionService.class);
    	startService(intent);
    }
    
    private void buttonDisconnectOnClick(View v)
    {
    	stopService(new Intent(this, ConnectionService.class));
    }
    
    private boolean verifySettings()
    {
    	SharedPreferences settings = getSharedPreferences(PREFERENCES_FILE, 0);
    	if (!settings.contains(PREF_MOUSE_PORT))
    		return false;
    	if (!settings.contains(PREF_KEYBOARD_PORT))
    		return false;
    	if (!settings.contains(PREF_SERVER_IP))
    		return false;
    	return true;
    }
}