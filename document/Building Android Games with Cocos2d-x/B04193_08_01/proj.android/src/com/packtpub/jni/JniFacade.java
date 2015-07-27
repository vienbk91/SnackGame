package com.packtpub.jni;

import org.cocos2dx.cpp.AppActivity;
import android.app.Activity;
import android.widget.Toast;

public class JniFacade {
	private static Activity activity = AppActivity.getInstance();
	
	public static void showToast(final String message) {
		activity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				Toast.makeText(activity.getBaseContext(), message, Toast.LENGTH_SHORT).show();	
			}
		});		
	}
}