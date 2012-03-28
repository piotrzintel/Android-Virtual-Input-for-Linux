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

import java.util.ArrayList;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Process;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ToggleButton;
import android.widget.CompoundButton.OnCheckedChangeListener;

public class KeyboardActivity extends Activity {
	private ConnectionReceiver mConnectionReceiver = new ConnectionReceiver();
	private ArrayList<Byte> switcherList;
	
	private Looper mAltQueueLooper;
	private AltQueueHandler mAltQueueHandler;
	
	private final class AltQueueHandler extends Handler {
		public AltQueueHandler(Looper looper) {
			super(looper);
		}
	}
	
	private class ConnectionReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (action.equals(ConnectionService.DISCONNECTED_INTENT) ||
					action.equals(ConnectionService.CONNECTION_LOST_INTENT)) {
				Intent i = new Intent(context, StartActivity.class);
				i.putExtra("ConnectionLost", true);
				startActivity(i);
			}
		}
		
	}
	
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.keyboard_screen);
        switcherList = new ArrayList<Byte>();
        
        HandlerThread thread = new HandlerThread("MouseConnectionServiceHandlerThread", Process.THREAD_PRIORITY_BACKGROUND);
		thread.start();
		mAltQueueLooper = thread.getLooper();
		mAltQueueHandler = new AltQueueHandler(mAltQueueLooper);
        
        Button button = (Button) findViewById(R.id.buttonSend);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonSendOnClick(v);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyTAB);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonTabOnClick(v, MessageTypes.KEY_TAB);
			}
		});
        
        
        button = (Button) findViewById(R.id.buttonKeyENTER);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_ENTER);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyESC);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_ESC);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyDELETE);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_DELETE);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyBACKSPACE);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_BACKSPACE);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyF1);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_F1);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyF2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_F2);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyF4);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_F4);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyF5);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_F5);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyHOME);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_HOME);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyEND);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_END);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyPGUP);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_PAGEUP);	
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyPGDN);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_PAGEDOWN);	
			}
		});
        
        ToggleButton togglebutton = (ToggleButton) findViewById(R.id.toggleButtonALT);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LALT);
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.ToggleButtonCTRL);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LCTRL);
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.ToggleButtonSHIFT);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LSHIFT);
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonMETA);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_META);
			}
		});
    }
	
	@Override
	public void onResume() {
		super.onResume();
		if (ConnectionService.mService == null) {
			finish();
		}
		IntentFilter iff = new IntentFilter();
    	iff.addAction(ConnectionService.DISCONNECTED_INTENT);
    	iff.addAction(ConnectionService.CONNECTION_FAILED_INTENT);
    	registerReceiver(mConnectionReceiver, iff);
	}
	
	@Override
    public void onPause()
    {
    	super.onPause();
    	unregisterReceiver(mConnectionReceiver);
    }
	
	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	MenuInflater inflater = getMenuInflater();
    	inflater.inflate(R.menu.chose_activity_menu, menu);
    	return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	Intent intent;
        switch (item.getItemId()) {
            case R.id.touchpadScreen:
            	intent = new Intent(this, TouchpadActivity.class);
            	startActivity(intent);
                return true;
                
            case R.id.startScreen:
                intent = new Intent(this, StartActivity.class);
                startActivity(intent);
                return true;
                
            case R.id.keyboardScreen:
            	return true;
                
            default:
                return super.onOptionsItemSelected(item);
        }
    }
	
	private boolean buttonSendOnClick(View v) {
		v.setPressed(true);
		final EditText et = (EditText) findViewById(R.id.editTextMessage);
		
		new Thread(new Runnable() {
			@Override
			public void run() {
				int i = 0;
				byte[] messagePre = new byte[switcherList.size()];
				for (Byte b : switcherList) {
					messagePre[i++] = b;
				}
				
				byte[] messagePost = new byte[switcherList.size()];
				i = messagePost.length;
				
				for (Byte b : switcherList) {
					messagePost[--i] = (byte)(b + 64);
				}
				
				try {
					ConnectionService.mService.sendKeyboardSpecial(messagePre);
					ConnectionService.mService.sendKeyboardText(et.getText().toString());
					ConnectionService.mService.sendKeyboardSpecial(messagePost);
				} catch (NullPointerException e) {
					Intent intent = new Intent(KeyboardActivity.this, ConnectionService.class);
					stopService(intent);
				}
			}
		}).run();
		
		et.setText("");
		v.setPressed(false);
		return true;
	}
	
	private boolean buttonOnClick(View v, byte key) {
		v.setPressed(true);
		int i = 0;
		byte[] message = new byte[2 + 2*switcherList.size()];
		for (Byte b : switcherList) {
			message[i++] = b;
		}
		message[i++] = key;
		message[i++] = (byte)(key + 64);
		i = message.length;
		for (Byte b : switcherList) {
			message[--i] = (byte)(b + 64);
		}
		try {
			ConnectionService.mService.sendKeyboardSpecial(message);
		} catch (NullPointerException e) {
			Intent intent = new Intent(this, ConnectionService.class);
			stopService(intent);
		}
		v.setPressed(false);
		return true;
	}
	
	private boolean buttonTabOnClick(View v, final byte key) {
		v.setPressed(true);
		int i = 0;
		byte[] message = new byte[2 + switcherList.size()];
		final byte[] messagePost = new byte[switcherList.size()];
		for (Byte b : switcherList) {
			message[i++] = b;
		}
		message[i++] = key;
		message[i++] = (byte)(key + 64);
		
		i = messagePost.length;
		for (Byte b : switcherList) {
			messagePost[--i] = (byte)(b + 64);
		}
		try {
			ConnectionService.mService.sendKeyboardSpecial(message);
		} catch (NullPointerException e) {
			Intent intent = new Intent(this, ConnectionService.class);
			stopService(intent);
		}
		
		mAltQueueHandler.removeCallbacksAndMessages(null);
		mAltQueueHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				try {
					ConnectionService.mService.sendKeyboardSpecial(messagePost);
				} catch (NullPointerException e) {
					Intent intent = new Intent(KeyboardActivity.this, ConnectionService.class);
					stopService(intent);
				}
			}
		}, 1800);
		
		v.setPressed(false);
		return true;
	}
	
	private void toggleButtonOnCheckChange(View v, boolean isChecked, byte key) {
		if ( isChecked ) {
			switcherList.add(new Byte(key));
		} else {
			switcherList.remove(new Byte(key));
		}
	}
}
