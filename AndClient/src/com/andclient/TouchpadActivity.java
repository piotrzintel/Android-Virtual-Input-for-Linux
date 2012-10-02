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
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Button;

public class TouchpadActivity extends Activity {
	private int mLastX;
	private int mLastY;
	
	private int mLastScrollX;
	private int mLastScrollY;
	
	private ConnectionReceiver mMouseConnectionReceiver = new ConnectionReceiver();
	
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
        setContentView(R.layout.touchpad_screen);
        
        mLastX = 0;
        mLastY = 0;
        
        View v = findViewById(R.id.touchPad);
        
        v.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return touchpadOnTouch(event);
			}
		});
        
        v = findViewById(R.id.viewTouchpadScrollVertical);
        
        v.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return touchpadScrollVerticalOnTouch(event);
			}
		});
        
        v = findViewById(R.id.viewTouchpadScrollHorizontal);
        
        v.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return touchpadScrollHorizontalOnTouch(event);
			}
		});
        
        Button button = (Button) findViewById(R.id.buttonTouchpadLMB);
        
        button.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return buttonMouseOnTouch(R.id.buttonTouchpadLMB, event, (byte)0);
			}
		});
        
        button = (Button) findViewById(R.id.buttonTouchpadRMB);
        
        button.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return buttonMouseOnTouch(R.id.buttonTouchpadRMB, event, (byte)1);
			}
		});
        
        button = (Button) findViewById(R.id.buttonTouchpadMMB);
        
        button.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return buttonMouseOnTouch(R.id.buttonTouchpadMMB, event, (byte)2);
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
    	registerReceiver(mMouseConnectionReceiver, iff);
	}
	
	@Override
    public void onPause()
    {
    	super.onPause();
    	unregisterReceiver(mMouseConnectionReceiver);
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
                return true;
                /*
            case R.id.startScreen:
                intent = new Intent(this, StartActivity.class);
                startActivity(intent);
                return true;
                */
            case R.id.keyboardScreen:
            	intent = new Intent(this, KeyboardActivity.class);
            	startActivity(intent);
            	return true;
                
            default:
                return super.onOptionsItemSelected(item);
        }
    }
	
	private boolean touchpadOnTouch(final MotionEvent event) {
		int Xvalue;
		int Yvalue;

		switch(event.getAction() & MotionEvent.ACTION_MASK) {
		case MotionEvent.ACTION_DOWN:
			mLastX = Math.round(event.getX());
			mLastY = Math.round(event.getY());
		break;
		
		case MotionEvent.ACTION_MOVE:
			Xvalue = Math.round(event.getX()) - mLastX;
			Yvalue = Math.round(event.getY()) - mLastY;
			Xvalue = Math.round((Xvalue*3)/2);
			Yvalue = Math.round((Yvalue*3)/2);
			
			try {
				ConnectionService.mService.sendMouseMove(Xvalue, Yvalue);
			} catch (NullPointerException e) {
				Intent intent = new Intent(this, ConnectionService.class);
				stopService(intent);
			}
			
			mLastX = Math.round(event.getX());
			mLastY = Math.round(event.getY());
		break;
		}
		return true;
	}
	
	private boolean buttonMouseOnTouch(int id, MotionEvent event, byte button) {
		View v = findViewById(id);
		int ev = event.getAction() & MotionEvent.ACTION_MASK;
		if ( ev == MotionEvent.ACTION_DOWN) {
			v.setPressed(true);
			try {
				ConnectionService.mService.sendMouseButtonPress(button);
			} catch (NullPointerException e) {
				Intent intent = new Intent(this, ConnectionService.class);
				stopService(intent);
			}
		} else if (ev == MotionEvent.ACTION_UP) {
			try {
				ConnectionService.mService.sendMouseButtonRelease(button);
			} catch (NullPointerException e) {
				Intent intent = new Intent(this, ConnectionService.class);
				stopService(intent);
			}
			v.setPressed(false);
		}
		return true;
	}
	
	private boolean touchpadScrollVerticalOnTouch( final MotionEvent event) {
		int Yvalue;

		switch(event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			mLastScrollY = Math.round(event.getY());
		break;
		
		case MotionEvent.ACTION_MOVE:  
			Yvalue = Math.round(event.getY()) - mLastScrollY;
			Yvalue = -Math.round(Yvalue/3);

			try {
				ConnectionService.mService.sendMouseScroll(MessageTypes.SCROLL_VERT, Yvalue);
			} catch (NullPointerException e) {
				Intent intent = new Intent(this, ConnectionService.class);
				stopService(intent);
			}

			mLastScrollY = Math.round(event.getY());
		break;
		}
		return true;
	}
	
	private boolean touchpadScrollHorizontalOnTouch( final MotionEvent event) {
		int Xvalue;

		switch(event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			mLastScrollX = Math.round(event.getX());
		break;
		
		case MotionEvent.ACTION_MOVE:  
			Xvalue = Math.round(event.getX()) - mLastScrollX;
			Xvalue = Math.round(Xvalue/3);

			try {
				ConnectionService.mService.sendMouseScroll(MessageTypes.SCROLL_HORIZ, Xvalue);
			} catch (NullPointerException e) {
				Intent intent = new Intent(this, ConnectionService.class);
				stopService(intent);
			}

			mLastScrollX = Math.round(event.getX());
		break;
		}
		return true;
	}
}
