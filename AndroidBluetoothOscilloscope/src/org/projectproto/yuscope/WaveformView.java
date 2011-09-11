package org.projectproto.yuscope;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class WaveformView extends SurfaceView implements SurfaceHolder.Callback{
	
	private WaveformPlotThread plot_thread;
	
	// plot area size
	private final int WIDTH = 320;
	private final int HEIGHT = 240;
	
	private Paint grid_paint = new Paint();
	private Paint cross_paint = new Paint();
	private Paint outline_paint = new Paint();

	public WaveformView(Context context, AttributeSet attrs){
		super(context, attrs);
		
		getHolder().addCallback(this);
		
		plot_thread = new WaveformPlotThread(getHolder(), this);
		
		grid_paint.setColor(Color.rgb(100, 100, 100));
		cross_paint.setColor(Color.rgb(70, 100, 70));
		outline_paint.setColor(Color.GREEN);
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height){
		
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder){
		plot_thread.setRunning(true);
		plot_thread.start();
	}
	
	@Override
	public void surfaceDestroyed(SurfaceHolder holder){
		boolean retry = true;
		plot_thread.setRunning(false);
		while (retry){
			try{
				plot_thread.join();
				retry = false;
			}catch(InterruptedException e){
				
			}
		}
	}
	
	@Override
	public void onDraw(Canvas canvas){
		PlotPoints(canvas);
	}
	
	public void PlotPoints(Canvas canvas){
		// clear screen
		canvas.drawColor(Color.rgb(20, 20, 20));
		// draw vertical grids
	    for(int vertical = 1; vertical<10; vertical++){
	    	canvas.drawLine(
	    			vertical*(WIDTH/10)+1, 1,
	    			vertical*(WIDTH/10)+1, HEIGHT+1,
	    			grid_paint);
	    }
	    // draw horizontal grids
	    for(int horizontal = 1; horizontal<10; horizontal++){
	    	canvas.drawLine(
	    			1, horizontal*(HEIGHT/10)+1,
	    			WIDTH+1, horizontal*(HEIGHT/10)+1,
	    			grid_paint);
	    }
	    // draw outline
 		canvas.drawLine(0, 0, (WIDTH+1), 0, outline_paint);	// top
 		canvas.drawLine((WIDTH+1), 0, (WIDTH+1), (HEIGHT+1), outline_paint); //right
 		canvas.drawLine(0, (HEIGHT+1), (WIDTH+1), (HEIGHT+1), outline_paint); // bottom
 		canvas.drawLine(0, 0, 0, (HEIGHT+1), outline_paint); //left
 		
 		// TODO: actual plotting of points
	}
}
