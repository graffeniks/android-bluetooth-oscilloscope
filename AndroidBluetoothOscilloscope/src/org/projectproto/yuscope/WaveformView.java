package org.projectproto.yuscope;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class WaveformView extends SurfaceView implements SurfaceHolder.Callback{

	public WaveformView(Context context, AttributeSet attrs){
		super(context, attrs);
		
		getHolder().addCallback(this);
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height){
		
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder){
		
	}
	
	@Override
	public void surfaceDestroyed(SurfaceHolder holder){
		
	}
	
	@Override
	public void onDraw(Canvas canvas){
		
	}
}
