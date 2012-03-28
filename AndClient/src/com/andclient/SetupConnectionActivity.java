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

import org.apache.http.conn.util.InetAddressUtils;

import android.app.Activity;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class SetupConnectionActivity extends Activity {
	private SharedPreferences mSettings;
	
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.config_screen);
        mSettings = getSharedPreferences(StartActivity.PREFERENCES_FILE, 0);
        
        EditText et = (EditText) findViewById(R.id.editTextServerIP);
        et.setText(mSettings.getString(StartActivity.PREF_SERVER_IP, ""));
        et = (EditText) findViewById(R.id.editTextMousePort);
        et.setText(Integer.toString(mSettings.getInt(StartActivity.PREF_MOUSE_PORT, 0)));
        et = (EditText) findViewById(R.id.editTextKeyboardPort);
        et.setText(Integer.toString(mSettings.getInt(StartActivity.PREF_KEYBOARD_PORT, 0)));
        
        Button button = (Button) findViewById(R.id.buttonSaveSettings);
        
        button.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				saveSettingsOnClick(v);
			}
		});
    }
	
	private void saveSettingsOnClick(View v)
	{		
		int keyboardPort = 0, mousePort = 0;
		
		EditText et = (EditText) findViewById(R.id.editTextServerIP);
		String ip = et.getText().toString();
		boolean correctIp = InetAddressUtils.isIPv4Address(ip);
		if (!correctIp) {
			Toast.makeText(SetupConnectionActivity.this, "Server IP is not a correct IPv4 address", 5).show();
		}
		
		et = (EditText) findViewById(R.id.editTextMousePort);
		String txt = et.getText().toString();
		if (txt.length() != 0) {
			try {
				mousePort = Integer.parseInt(txt);
			} catch (NumberFormatException e) {
				Toast.makeText(SetupConnectionActivity.this, "Mouse port is not a number", 5).show();
			}
			if (mousePort < 0) {
				Toast.makeText(SetupConnectionActivity.this, "Mouse port must be greater than 0", 5).show();
			}
		}
		
		et = (EditText) findViewById(R.id.editTextKeyboardPort);
		txt = et.getText().toString();
		if (txt.length() != 0) {
			try {
				keyboardPort = Integer.parseInt(txt);
			} catch (NumberFormatException e) {
				Toast.makeText(SetupConnectionActivity.this, "Keyboard port must be an integer", 5).show();
			}
			if (keyboardPort < 0) {
				Toast.makeText(SetupConnectionActivity.this, "Keyboard port must be greater than 0", 5).show();
			}
		}
		
		if (correctIp && mousePort > 0 && keyboardPort > 0 )
		{
			SharedPreferences.Editor editor = mSettings.edit();
			editor.putString(StartActivity.PREF_SERVER_IP, ip)
				  .putInt(StartActivity.PREF_MOUSE_PORT, mousePort)
				  .putInt(StartActivity.PREF_KEYBOARD_PORT, keyboardPort);
			
			editor.commit();
			TextView tv = (TextView) findViewById(R.id.textViewSaveData);
			tv.setTextColor(Color.GREEN);
			tv.setText("Settings saved.");
			tv.setVisibility(View.VISIBLE);
		}
		else
		{
			TextView tv = (TextView) findViewById(R.id.textViewSaveData);
			tv.setText("Incorrect input data.");
			tv.setTextColor(Color.RED);
			tv.setVisibility(View.VISIBLE);
		}
	}
}
