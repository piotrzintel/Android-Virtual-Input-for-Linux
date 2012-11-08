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
import android.widget.ViewAnimator;

public class KeyboardActivity extends Activity {
	private ConnectionReceiver mConnectionReceiver = new ConnectionReceiver();
	private ArrayList<Byte> mSwitcherList;
	
	private Looper mAltQueueLooper;
	private AltQueueHandler mAltQueueHandler;
	
	private ViewAnimator mKeyboardViewAnimator;
	private int mDisplayedChild;
	
	private static final class AltQueueHandler extends Handler {
		//private final WeakReference<>
		
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
        mSwitcherList = new ArrayList<Byte>();
        
        prepareLayoutAnimator();
        
        prepareButtonListeners();
        
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
                
            case R.id.keyboardScreen:
            	return true;
                
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public Object onRetainNonConfigurationInstance() {
    	return Integer.valueOf(mDisplayedChild);
    }
    
	private boolean buttonSendOnClick(View v) {
		v.setPressed(true);
		final EditText et = (EditText) findViewById(R.id.editTextMessage);
		
		new Thread(new Runnable() {
			@Override
			public void run() {
				int i = 0;
				byte[] messagePre = new byte[mSwitcherList.size()];
				for (Byte b : mSwitcherList) {
					messagePre[i++] = b;
				}
				
				byte[] messagePost = new byte[mSwitcherList.size()];
				i = messagePost.length;
				
				for (Byte b : mSwitcherList) {
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
		byte[] message = new byte[2 + 2*mSwitcherList.size()];
		for (Byte b : mSwitcherList) {
			message[i++] = b;
		}
		message[i++] = key;
		message[i++] = (byte)(key + 64);
		i = message.length;
		for (Byte b : mSwitcherList) {
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
		byte[] message = new byte[2 + mSwitcherList.size()];
		final byte[] messagePost = new byte[mSwitcherList.size()];
		for (Byte b : mSwitcherList) {
			message[i++] = b;
		}
		message[i++] = key;
		message[i++] = (byte)(key + 64);
		
		i = messagePost.length;
		for (Byte b : mSwitcherList) {
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
		if ( isChecked && !(mSwitcherList.contains(new Byte(key))) ) {
			mSwitcherList.add(new Byte(key));
		} else if (!isChecked) {
			final byte[] keyOff = new byte[1];
			keyOff[0] = (byte)(key + 64);

			mAltQueueHandler.post( new Runnable() {
				
				@Override
				public void run() {
					try {
						ConnectionService.mService.sendKeyboardSpecial(keyOff);
					} catch (NullPointerException e) {
						Intent intent = new Intent(KeyboardActivity.this, ConnectionService.class);
						stopService(intent);
					}
				}
				
			});
			
			mSwitcherList.remove(new Byte(key));
		}
	}
	
	private void prepareLayoutAnimator() {
        mKeyboardViewAnimator = new ViewAnimator(this);
        
        View v1 = View.inflate(this, R.layout.keyboard_screen_1, null);
        View v2 = View.inflate(this, R.layout.keyboard_screen_2, null);
        
        mKeyboardViewAnimator.addView(v1, 0);
        mKeyboardViewAnimator.addView(v2, 1);
        
        Object lastNonConfigurationInstance = getLastNonConfigurationInstance();
        if ( lastNonConfigurationInstance != null ) {
        	mDisplayedChild = (Integer)lastNonConfigurationInstance;
        	mKeyboardViewAnimator.setDisplayedChild(mDisplayedChild);
        } else {
        	mDisplayedChild = mKeyboardViewAnimator.getDisplayedChild();
        }
        
        setContentView(mKeyboardViewAnimator);
	}
	
	private void prepareButtonListeners() {
		
        HandlerThread thread = new HandlerThread("KeyboardAltButtonTurnOffThread", Process.THREAD_PRIORITY_BACKGROUND);
		thread.start();
		mAltQueueLooper = thread.getLooper();
		mAltQueueHandler = new AltQueueHandler(mAltQueueLooper);
        
		///////////////////////
		// FIRST SCREEN
		
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

        button = (Button) findViewById(R.id.buttonNextKeyboard);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mDisplayedChild++;
				mDisplayedChild %= 2;
				
				mKeyboardViewAnimator.setDisplayedChild(mDisplayedChild);
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
				( (ToggleButton) findViewById(R.id.toggleButtonALT2) ).setChecked(buttonView.isChecked());
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonCTRL);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LCTRL);
				( (ToggleButton) findViewById(R.id.toggleButtonCTRL2) ).setChecked(buttonView.isChecked());
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonSHIFT);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LSHIFT);
				( (ToggleButton) findViewById(R.id.toggleButtonSHIFT2) ).setChecked(buttonView.isChecked());
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonMETA);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_META);
				( (ToggleButton) findViewById(R.id.toggleButtonMETA2) ).setChecked(buttonView.isChecked());
			}
		});
        
        // END FIRST SCREEN
        
        ///////////////////////
        // SECOND SCREEN
        
        button = (Button) findViewById(R.id.buttonNextKeyboard2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mDisplayedChild++;
				mDisplayedChild %= 2;
				
				mKeyboardViewAnimator.setDisplayedChild(mDisplayedChild);
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonALT2);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LALT);
				( (ToggleButton) findViewById(R.id.toggleButtonALT) ).setChecked(buttonView.isChecked());
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonCTRL2);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LCTRL);
				( (ToggleButton) findViewById(R.id.toggleButtonCTRL) ).setChecked(buttonView.isChecked());
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonSHIFT2);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_LSHIFT);
				( (ToggleButton) findViewById(R.id.toggleButtonSHIFT) ).setChecked(buttonView.isChecked());
			}
		});
        
        togglebutton = (ToggleButton) findViewById(R.id.toggleButtonMETA2);
        
        togglebutton.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				toggleButtonOnCheckChange(buttonView, isChecked, MessageTypes.KEY_META);
				( (ToggleButton) findViewById(R.id.toggleButtonMETA) ).setChecked(buttonView.isChecked());
			}
		});
                
        button = (Button) findViewById(R.id.buttonKeyTAB2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonTabOnClick(v, MessageTypes.KEY_TAB);
			}
		});
                
        button = (Button) findViewById(R.id.buttonKeyENTER2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_ENTER);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyPrev);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_PREVIOUSSONG);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyPlayPause);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_PLAYPAUSE);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyNext);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_NEXTSONG);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyMute);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_MUTE);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyVolDown);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_VOLUMEDOWN);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyVolUp);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_VOLUMEUP);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyHome2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_HOME);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyUp);
        button.setText("/\\");
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_UP);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyEnd2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_END);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyPgUp2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_PAGEUP);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyLeft);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_LEFT);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyDown);
        button.setText("\\/");
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_DOWN);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyRight);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_RIGHT);
			}
		});
        
        button = (Button) findViewById(R.id.buttonKeyPgDn2);
        
        button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				buttonOnClick(v, MessageTypes.KEY_PAGEDOWN);
			}
		});
	}
}
