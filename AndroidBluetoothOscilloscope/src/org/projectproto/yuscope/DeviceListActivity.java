package org.projectproto.yuscope;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class DeviceListActivity extends Activity {
	// Return Intent extra
    public static String EXTRA_DEVICE_ADDRESS = "device_address";
    
    @Override
    protected void onCreate(Bundle savedInstanceState){
    	super.onCreate(savedInstanceState);
    	
    	// Setup the window
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.device_list);
        
        // Set result CANCELED incase the user backs out
        setResult(Activity.RESULT_CANCELED);
        
        // Initialize the button to perform device discovery
        Button scanButton = (Button) findViewById(R.id.button_scan);
        scanButton.setOnClickListener(new OnClickListener(){
        	public void onClick(View v){
        		doDiscovery();
        		v.setVisibility(View.GONE);
        	}
        });
    }
    
    /**
     * Start device discover with the BluetoothAdapter
     */
    private void doDiscovery(){
    	Toast.makeText(this, "todo: discovering remote devices", Toast.LENGTH_LONG).show();
    }
}
