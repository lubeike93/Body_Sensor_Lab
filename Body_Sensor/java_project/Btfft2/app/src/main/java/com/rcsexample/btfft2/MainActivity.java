package com.rcsexample.btfft2;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Vibrator;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.GridLayout;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.widget.TextView;

import com.androidplot.ui.LayoutManager;
import com.androidplot.ui.SizeLayoutType;
import com.androidplot.ui.SizeMetrics;
import com.androidplot.xy.FillDirection;
import com.androidplot.xy.LineAndPointFormatter;
import com.androidplot.xy.SimpleXYSeries;
import com.androidplot.xy.XLayoutStyle;
import com.androidplot.xy.XYGraphWidget;
import com.androidplot.xy.XYPlot;
import com.androidplot.xy.YLayoutStyle;
import com.rcsexample.btfft2.R;
import com.rcsexample.bsnlib.BluetoothConnectionService;
import com.rcsexample.bsnlib.BluetoothConnectionService.BluetoothConnectionListener;
import com.rcsexample.bsnlib.ControlPacket;
import com.rcsexample.bsnlib.Data;
import com.rcsexample.bsnlib.DataProvider;
import com.rcsexample.bsnlib.DataProvider.OnDataAvailableListener;
import com.rcsexample.bsnlib.DeviceListActivity;

import org.w3c.dom.Text;

/**
 * This is the Activity class
 *
 * @author laurenz
 *
 */

//<style name="AppBaseTheme" parent="android:Theme.Light.NoTitleBar">

public class MainActivity extends Activity implements OnItemSelectedListener, BluetoothConnectionListener {
    /** This is the MAC address of the bluetooth sensor. */
    private String deviceAddress = "00:80:25:01:57:F6";

    /** Maximum number of points in the upper chart. Affects performance. */
    private static final int PLOT_HISTORY_SIZE = 128;

    /** The speed of phone sensor. Affects battery and performance. */
    private static final int SENSOR_DELAY = SensorManager.SENSOR_DELAY_FASTEST;

    /** Intent request code for DeviceListActivity */
    private static final int INTENT_REQUEST_CHOOSE_DEVICE = 1;

    /** The input selection dropdown */
    private Spinner spinner = null;
    /** vibrator button */
    private ToggleButton btnVibrate = null;
    /** bt send button */
    private ToggleButton btnSend = null;
    /** bt other Command Button */
    private Button btnCmd = null;

    // MINE
    private MotionClassifier motionClassifier = null;
    private MotionClassifier fullFeatureMotionClassifier = null;
    private FeatureBot featureBot = null;

    private Button resetStepButton = null;
    private TextView statusTextView = null;
    private TextView stepsTextView = null;
    private TextView calV = null;
    private TextView startStopStepButton = null;
    private Button switchModelButton = null;

    private TextView tminus2Value = null;
    private TextView tminus1Value = null;
    private TextView tValue = null;
    private TextView tplus1Value = null;

    private TextView maxSVMTV = null;
    private TextView maxXTV = null;
    private TextView minXTV = null;
    private TextView stdSVMTV = null;
    private TextView stdXTV = null;
    private TextView meanPeakTV = null;

    private TextView maxSVMValue = null;
    private TextView maxXValue = null;
    private TextView minXValue = null;
    private TextView stdSVMValue = null;
    private TextView stdXValue = null;
    private TextView meanPeakValue = null;

    private GridLayout gap7 = null;
    private GridLayout gap9 = null;
    private LinearLayout featureValuesLayout = null;
    private LinearLayout statesLayout = null;
    private Button debugButton = null;

    private Button switchStepButton = null;
    private GridLayout gap10 = null;

    // flags
    private int resetClicked = 0;
    private int lastState = -1;
    private boolean tickTock = true;
    private boolean useFullFeat = true;
    private static int stepCount = 0;
    private static double calCount = 0.0;
    private static boolean isCountingSteps = false;
    private boolean mrIsBusy = false;
    private boolean isDebug = false;
    private boolean isRealTimeCounting = true;

    private ArrayList<Integer> motionStata = new ArrayList<>();
    private ArrayList<Integer> motionStataProtocol = new ArrayList<>();
    private ArrayList<Double> countX = new ArrayList<>();
    private ArrayList<Double> countY = new ArrayList<>();
    private ArrayList<Double> countZ = new ArrayList<>();

    private static int motionStatus = -1; // -1 for standby, 0 for walk, 1 for run, 2 for jump
    private static final String MODEL_FILE = "file:///android_asset/6feats_mixswami_chest_model.pb";
    private static final String FULL_FEAT_MODEL_FILE = "file:///android_asset/17feats_remix_chest_model.pb";
    private static final int FEATURE_NUM = 6;
    private static final String INPUT_NAME = "dense_1_input";
    private static final String OUTPUT_NAME = "output_node0";

    // transition smoothing
    private static final int[] TRANSITION_SMOOTHING_3TO2_1 = new int[] {3, 3, 1, 2};
    private static final int[] TRANSITION_SMOOTHING_3TO2_2 = new int[] {3, 3, 0, 2};
    private static final int[] TRANSITION_SMOOTHING_3TO2_3 = new int[] {3, 0, 1, 2};
    private static final int[] TRANSITION_SMOOTHING_2TO3_1 = new int[] {2, 2, 1, 3};
    private static final int[] TRANSITION_SMOOTHING_2TO3_2 = new int[] {2, 2, 0, 3};
    private static final int[] TRANSITION_SMOOTHING_2TO3_3 = new int[] {2, 1, 0, 3};

    private static final int[] TRANSITION_SMOOTHING_0TO2 = new int[] {0, 0, 1, 2};
    private static final int[] TRANSITION_SMOOTHING_2TO0 = new int[] {2, 2, 1, 0};

    private static final int[] JUMPING_SMOOTHING_1 = new int[] {2, 1, 1, 2};
    private static final int[] JUMPING_SMOOTHING_2 = new int[] {1, 2, 1, 2};
    private static final int[] JUMPING_SMOOTHING_3 = new int[] {2, 1, 0, 2};
    private static final int[] JUMPING_SMOOTHING_4 = new int[] {2, 0, 1, 2};

    // END MINE

    /** The upper chart */
    private XYPlot accPlot = null;
    /** chart point series for upper chart, X dimension */
    private SimpleXYSeries accSeriesX = null;
    /** chart point series for upper chart, Y dimension */
    private SimpleXYSeries accSeriesY = null;
    /** chart point series for upper chart, Z dimension */
    private SimpleXYSeries accSeriesZ = null;
    /* The lower chart
    private XYPlot freqPlot = null;
     chart point series for lower chart, X dimension
    private SimpleXYSeries freqSeriesX = new SimpleXYSeries("X");
     chart point series for lower chart, Y dimension
    private SimpleXYSeries freqSeriesY = null;
     chart point series for lower chart, Z dimension
    private SimpleXYSeries freqSeriesZ = null; */

    /** Data values (again) for passing to JNI. X dimension. */
    private ArrayList<Double> accX = new ArrayList<Double>(PLOT_HISTORY_SIZE);
    /** Data values (again) for passing to JNI. Y dimension. */
    private ArrayList<Double> accY = new ArrayList<Double>(PLOT_HISTORY_SIZE);
    /** Data values (again) for passing to JNI. Z dimension. */
    private ArrayList<Double> accZ = new ArrayList<Double>(PLOT_HISTORY_SIZE);

    /** The DataProvider object provides data values from various sources */
    private DataProvider dataProvider = null;
    /** The BluetoothConnectionService handles the connection */
    private BluetoothConnectionService bluetoothService;

    /** This is the local bluetooth module */
    private BluetoothAdapter bluetoothAdapter = null;
    /** This is the remote bluetooth device (the sensor) */
    private BluetoothDevice device = null;

    /** Phone vibrator */
    private Vibrator vibrator = null;

    /** This flag is used to prevent fft calculation overflow */
    private boolean fftIsBusy = false;

    /** This flag is used to recall the provider-state in onPause/onResume */
    private boolean phoneWasProviding = false;
    /** This flag is used to recall the provider-state in onPause/onResume */
    private boolean bluetoothWasProviding = false;

    /** Used to display file chooser on file input */
    private ArrayList<String> files = null;
/*
    static {
        System.loadLibrary("ndkmodule");
    }*/
/*
    private native void initializeModel();

    private native void stepModel();

    private native void setModelInput(double val, int i);

    private native double getModelOutput(int i);
*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // set up the spinner
        spinner = (Spinner) findViewById(R.id.spinner);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.connections_array, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);

        // set up the plots
        accPlot = (XYPlot) findViewById(R.id.accPlot);
        // freqPlot = (XYPlot) findViewById(R.id.freqPlot);
        initPlot();

        // set up engines
        dataProvider = new DataProvider(getApplicationContext());
        bluetoothService = new BluetoothConnectionService();
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        dataProvider.registerListener(onDataAvailableListener);
        bluetoothService.registerListener(this);
        spinner.setOnItemSelectedListener(this);

        // set up button functionality
        vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        btnVibrate = (ToggleButton) findViewById(R.id.btnVibrate);
        btnSend = (ToggleButton) findViewById(R.id.btnSend);
        btnCmd = (Button) findViewById(R.id.btnCmd);
        btnCmd.setOnClickListener(onClickListener);
        btnVibrate.setOnCheckedChangeListener(onCheckedChangeListener);
        btnSend.setOnCheckedChangeListener(onCheckedChangeListener);

        // set up lower part
        statusTextView = (TextView) findViewById(R.id.statusTextView);
        stepsTextView = (TextView) findViewById(R.id.stepsV);
        calV = findViewById(R.id.calV);
        resetStepButton = (Button) findViewById(R.id.resetStepButton);
        resetStepButton.setOnClickListener(onClickListener);
        buttonEffect(resetStepButton);
        startStopStepButton = (Button) findViewById(R.id.startStopStepButton);
        startStopStepButton.setOnClickListener(onClickListener);
        buttonEffect(startStopStepButton);
        switchModelButton = (Button) findViewById(R.id.switchModelButton);
        switchModelButton.setOnClickListener(onClickListener);
        switchModelButton.setText("BIG");

        maxSVMTV = (TextView) findViewById(R.id.maxSVMTV);
        maxXTV = (TextView) findViewById(R.id.maxXTV);
        minXTV = (TextView) findViewById(R.id.minXTV);
        stdSVMTV = (TextView) findViewById(R.id.stdSVMTV);
        stdXTV = (TextView) findViewById(R.id.stdXTV);
        meanPeakTV = (TextView) findViewById(R.id.meanPeakTV);

        tminus1Value = (TextView) findViewById(R.id.tminus1Value);
        tminus2Value = (TextView) findViewById(R.id.tminus2Value);
        tValue = (TextView) findViewById(R.id.tValue);
        tplus1Value = (TextView) findViewById(R.id.tplus1Value);

        maxSVMValue = (TextView) findViewById(R.id.maxSVMValue);
        maxXValue = (TextView) findViewById(R.id.maxXValue);
        minXValue = (TextView) findViewById(R.id.minXValue);
        stdSVMValue = (TextView) findViewById(R.id.stdSVMValue);
        stdXValue = (TextView) findViewById(R.id.stdXValue);
        meanPeakValue = (TextView) findViewById(R.id.meanPeakValue);

        // find debug layouts
        gap7 = (GridLayout) findViewById(R.id.gap7);
        gap7.setVisibility(View.GONE);
        gap9 = (GridLayout) findViewById(R.id.gap9);
        gap9.setVisibility(View.GONE);
        featureValuesLayout = (LinearLayout) findViewById(R.id.featureValuesLayout);
        featureValuesLayout.setVisibility(View.GONE);
        statesLayout = (LinearLayout) findViewById(R.id.statesLayout);
        statesLayout.setVisibility(View.GONE);
        debugButton = (Button) findViewById(R.id.debugButton);
        debugButton.setOnClickListener(onClickListener);
        buttonEffect(debugButton);
        gap10 = (GridLayout) findViewById(R.id.gap10);
        gap10.setVisibility(View.GONE);
        switchStepButton = (Button) findViewById(R.id.switchStepButton);
        switchStepButton.setOnClickListener(onClickListener);
        switchStepButton.setVisibility(View.GONE);
        buttonEffect(switchStepButton);

        try {
            motionClassifier = MotionClassifier.create(getAssets(), MODEL_FILE, INPUT_NAME, OUTPUT_NAME, FEATURE_NUM);
            fullFeatureMotionClassifier = MotionClassifier.create(getAssets(), FULL_FEAT_MODEL_FILE, INPUT_NAME, OUTPUT_NAME, 17);
            featureBot = new FeatureBot();
        } catch (Exception e) {
            makeToast(e.getMessage());
        }
    }

    @Override
    protected void onPause() {

        // App becomes (partly) invisible. Stop receiving data and save states.
        phoneWasProviding = dataProvider.isPhoneProviding();
        bluetoothWasProviding = dataProvider.isBluetoothProviding();
        dataProvider.disableBluetoothSensorProviding();
        dataProvider.disablePhoneSensorProviding();

        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();

        // App is visible again.

        if (bluetoothWasProviding) dataProvider.enableBluetoothSensorProviding(bluetoothService);
        if (phoneWasProviding) dataProvider.enablePhoneSensorProviding(SENSOR_DELAY);

    }

    /** builds a dialog box that asks whether to enable bluetooth */
    private void askToEnableBT() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this).setTitle("Bluetooth turned off")
                .setMessage("Do you want to enable Bluetooth and try to connect?").setPositiveButton("YES", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        bluetoothAdapter.enable();
                        initBluetoothCon();
                        dialog.dismiss();
                    }
                }).setNegativeButton("NO", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
        builder.create().show();
    }

    @Override
    protected void onStop() {
        // kill bluetooth connection when App is completely hidden

        bluetoothService.stop();

        super.onStop();
    }

    @Override
    protected void onStart() {
        // If we killed the connection on onStop() we should reconnect now.

        if (bluetoothWasProviding) initBluetoothCon();

        super.onStart();
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == INTENT_REQUEST_CHOOSE_DEVICE) {
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                deviceAddress = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                if (BluetoothAdapter.checkBluetoothAddress(deviceAddress)) {
                    device = bluetoothAdapter.getRemoteDevice(deviceAddress);
                    bluetoothService.connect(device);
                    dataProvider.enableBluetoothSensorProviding(bluetoothService);
                } else {
                    makeToast("BT address \"" + deviceAddress + "\" is invalid");
                }
            }
        }
    }

    public static void buttonEffect(View button){
        button.setOnTouchListener(new View.OnTouchListener() {

            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN: {
                        v.getBackground().setColorFilter(0xe0f47521, PorterDuff.Mode.SRC_ATOP);
                        v.invalidate();
                        break;
                    }
                    case MotionEvent.ACTION_UP: {
                        v.getBackground().clearColorFilter();
                        v.invalidate();
                        break;
                    }
                }
                return false;
            }
        });
    }

    /**
     * gets called when a button is clicked
     */
    private OnClickListener onClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            if (v.equals(btnCmd)) {
                AlertDialog.Builder alert = new AlertDialog.Builder(MainActivity.this);

                alert.setTitle("Send BT Cmd");
                alert.setMessage("Type the commands you want to send to the BT sensor");

                // Set an EditText view to get user input
                final EditText input = new EditText(MainActivity.this);
                alert.setView(input);

                alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        String value = input.getText().toString();
                        bluetoothService.write(value.getBytes());
                    }
                });

                alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        // Canceled.
                    }
                });

                alert.show();
            } else if (v.equals(resetStepButton)) {
                if (stepCount == 0) resetClicked++;
                else resetClicked = 0;
                if (resetClicked == 4) {
                    //TODO easter egg

                    resetClicked = 0;
                } else {
                    stepCount = 0;
                    calCount = 0;
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            stepsTextView.setText(String.valueOf(stepCount));
                            calV.setText(String.valueOf(calCount));
                        }
                    });
                }
            } else if (v.equals(switchModelButton)) {
                useFullFeat = !useFullFeat;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (useFullFeat) switchModelButton.setText("BIG");
                        else switchModelButton.setText("little");
                    }
                });
            } else if (v.equals(startStopStepButton)) {
                if (!isRealTimeCounting && isCountingSteps) {
                    double[] x = arrayList2Array(countX);
                    double[] y = arrayList2Array(countY);
                    double[] z = arrayList2Array(countZ);
                    countX.clear();
                    countY.clear();
                    countZ.clear();

                    stepCount += featureBot.getSteps(x, y, z, 10);
                    calCount = 66 * stepCount / 1000;
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            stepsTextView.setText(String.valueOf(stepCount));
                            calV.setText(String.valueOf(calCount));
                        }
                    });
                }
                isCountingSteps = !isCountingSteps;
            } else if (v.equals(debugButton)) {
                if (!isDebug) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            featureValuesLayout.setVisibility(View.VISIBLE);
                            statesLayout.setVisibility(View.VISIBLE);
                            gap7.setVisibility(View.VISIBLE);
                            gap9.setVisibility(View.VISIBLE);
                            gap10.setVisibility(View.VISIBLE);
                            switchStepButton.setVisibility(View.VISIBLE);
                        }
                    });
                    isDebug = true;
                } else {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            featureValuesLayout.setVisibility(View.GONE);
                            statesLayout.setVisibility(View.GONE);
                            gap7.setVisibility(View.GONE);
                            gap9.setVisibility(View.GONE);
                            gap10.setVisibility(View.GONE);
                            switchStepButton.setVisibility(View.GONE);
                        }
                    });
                    isDebug = false;
                }

            } else if (v.equals(switchStepButton)) {
                if (isRealTimeCounting) {

                    isRealTimeCounting = false;
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            switchStepButton.setText("Switch to Real-Time mode");
                        }
                    });
                } else {

                    isRealTimeCounting = true;
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            switchStepButton.setText("Switch to ranged mode");
                        }
                    });
                }
            }

        }
    };

    /**
     * gets called when one of the toggle buttons is clicked
     */
    private OnCheckedChangeListener onCheckedChangeListener = new OnCheckedChangeListener() {
        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            if (buttonView.equals(btnVibrate)) {
                if (isChecked) {
                    // start vibrating forever
                    vibrator.vibrate(new long[] { 0, 100 }, 0);
                } else {
                    vibrator.cancel();
                }
            } else if (buttonView.equals(btnSend)) {
                if (isChecked) {
                    new ControlPacket(ControlPacket.INSTRUCTION_START_STOP_MEASURE_SEND_BT).build().send(bluetoothService);
                } else {
                    new ControlPacket(ControlPacket.INSTRUCTION_SLEEP).build().send(bluetoothService);
                }
            }
        }
    };

    /**
     * The method in this listener gets called when the DataProvider has new
     * Data available.
     */
    private final OnDataAvailableListener onDataAvailableListener = new OnDataAvailableListener() {
        @Override
        public synchronized void onDataAvailable(Data data) {
            // add the samples in data to the plots and our value arrays
            for (int i = 0; i < data.getNrOfSamples(); i++) {
                accSeriesX.addLast(data.getT().get(i), data.getX().get(i));
                accSeriesY.addLast(data.getT().get(i), data.getY().get(i));
                accSeriesZ.addLast(data.getT().get(i), data.getZ().get(i));

                accX.addAll(data.getX());
                accY.addAll(data.getY());
                accZ.addAll(data.getZ());
            }

            // trim plots to maximum size
            while (accSeriesX.size() > PLOT_HISTORY_SIZE)
                accSeriesX.removeFirst();
            while (accSeriesY.size() > PLOT_HISTORY_SIZE)
                accSeriesY.removeFirst();
            while (accSeriesZ.size() > PLOT_HISTORY_SIZE)
                accSeriesZ.removeFirst();
            while (accX.size() > PLOT_HISTORY_SIZE)
                accX.remove(0);
            while (accY.size() > PLOT_HISTORY_SIZE)
                accY.remove(0);
            while (accZ.size() > PLOT_HISTORY_SIZE)
                accZ.remove(0);

            // calculate the spectrum
            // getFrequencies();

            try {
                getMotionStatus();
                final String statusString = parseMotionStatus();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        statusTextView.setText(statusString);
                        stepsTextView.setText(String.valueOf(stepCount));
                    }
                });
            } catch (Exception e) {
                makeToast(e.getMessage());
            }

            // update the plot
            accPlot.redraw();
            tickTock = !tickTock;
        }
    };

    private double getMax(ArrayList<Double> x){
        double max = -999999.0;
        for (double num : x) {
            if (max < num) {
                max = num;
            }
        }
        return max;
    }

    /**
     * sets up plot style and content
     */
    private void initPlot() {
        ArrayList<XYPlot> plotList = new ArrayList<XYPlot>(2);
        plotList.add(accPlot);
        // plotList.add(freqPlot);
        for (XYPlot plot : plotList) {
            LayoutManager lm = plot.getLayoutManager();
            XYGraphWidget gw = plot.getGraphWidget();
            lm.remove(plot.getDomainLabelWidget());
            lm.remove(plot.getRangeLabelWidget());
            lm.remove(plot.getTitleWidget());
            lm.remove(plot.getLegendWidget());
            lm.position(gw, 0, XLayoutStyle.ABSOLUTE_FROM_LEFT, 0, YLayoutStyle.ABSOLUTE_FROM_TOP);
            gw.setSize(new SizeMetrics(0, SizeLayoutType.FILL, 0, SizeLayoutType.FILL));
            gw.getBackgroundPaint().setColor(Color.TRANSPARENT);
            gw.setGridBackgroundPaint(null);
            gw.getGridLinePaint().setColor(Color.TRANSPARENT);
            gw.setDomainLabelPaint(null);
            gw.setDomainOriginLabelPaint(null);
            // plot.setDrawBorderEnabled(false);
            plot.setBackgroundPaint(null);
            /*
            try {
                //Drawable gundumPic = getResources().getDrawable( R.drawable.rx78, null );
                plot.getBackgroundPaint().setColor(Color.TRANSPARENT);
                //plot.setBackground(gundumPic);
            } catch (Exception e) {
                makeToast(e.getMessage());
            }*/
        }

        accSeriesX = new SimpleXYSeries("X");
        accSeriesY = new SimpleXYSeries("Y");
        accSeriesZ = new SimpleXYSeries("Z");
        /* freqSeriesX = new SimpleXYSeries("X");
        freqSeriesY = new SimpleXYSeries("Y");
        freqSeriesZ = new SimpleXYSeries("Z"); */

        for (int i = 0; i < PLOT_HISTORY_SIZE; i++) {
            accX.add(Double.valueOf(0));
            accY.add(Double.valueOf(0));
            accZ.add(Double.valueOf(0));
        }
        /* for (int i = 0; i < PLOT_HISTORY_SIZE / 2; i++) {
            freqSeriesX.addLast(i, Double.valueOf(0));
            freqSeriesY.addLast(i, Double.valueOf(0));
            freqSeriesZ.addLast(i, Double.valueOf(0));
        } */
        accPlot.addSeries(accSeriesX, new LineAndPointFormatter(Color.RED, null, null, null, FillDirection.BOTTOM));
        accPlot.addSeries(accSeriesY, new LineAndPointFormatter(Color.BLUE, null, null, null, FillDirection.BOTTOM));
        accPlot.addSeries(accSeriesZ, new LineAndPointFormatter(Color.YELLOW, null, null, null, FillDirection.BOTTOM));
        /* freqPlot.addSeries(freqSeriesX, new LineAndPointFormatter(Color.RED, null, null, null, FillDirection.BOTTOM));
        freqPlot.addSeries(freqSeriesY, new LineAndPointFormatter(Color.BLUE, null, null, null, FillDirection.BOTTOM));
        freqPlot.addSeries(freqSeriesZ, new LineAndPointFormatter(Color.GREEN, null, null, null, FillDirection.BOTTOM)); */
    }

    /**
     * gets called when the input selection dropdown was used
     */
    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
        switch (pos) {
            case Data.DATA_SOURCE_NONE:
                bluetoothService.stop();
                dataProvider.disableBluetoothSensorProviding();
                dataProvider.disablePhoneSensorProviding();
                dataProvider.disableFileProviding();
                btnVibrate.setVisibility(View.GONE);
                btnCmd.setVisibility(View.GONE);
                btnSend.setVisibility(View.GONE);
                break;
            case Data.DATA_SOURCE_PHONE:
                bluetoothService.stop();
                dataProvider.disableBluetoothSensorProviding();
                dataProvider.disableFileProviding();
                clearData();
                dataProvider.enablePhoneSensorProviding(SENSOR_DELAY);
                btnVibrate.setVisibility(View.VISIBLE);
                btnCmd.setVisibility(View.GONE);
                btnSend.setVisibility(View.GONE);
                break;
            case Data.DATA_SOURCE_BLUETOOTH:
                dataProvider.disablePhoneSensorProviding();
                dataProvider.disableFileProviding();
                clearData();
                initBluetoothCon();
                btnVibrate.setVisibility(View.GONE);
                //btnCmd.setVisibility(View.VISIBLE);
                btnSend.setVisibility(View.VISIBLE);
                break;
            case Data.DATA_SOURCE_FILE:
                dataProvider.disableBluetoothSensorProviding();
                dataProvider.disablePhoneSensorProviding();
                clearData();
                initFileReading();
                btnVibrate.setVisibility(View.GONE);
                btnCmd.setVisibility(View.GONE);
                btnSend.setVisibility(View.GONE);
                break;
        }
    }

    private synchronized void clearData() {
        while(accSeriesX.size()>0) accSeriesX.removeLast();
        while(accSeriesY.size()>0) accSeriesY.removeLast();
        while(accSeriesZ.size()>0) accSeriesZ.removeLast();
    }

    private void initFileReading() {
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            String[] downloadDirFiles = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).list();
            if (downloadDirFiles == null) {
                makeToast("Download directory not found");
            } else {
                files = new ArrayList<String>(downloadDirFiles.length);
                for (int i = 0; i < downloadDirFiles.length; i++) {
                    if (downloadDirFiles[i].endsWith(".csv")) files.add(downloadDirFiles[i]);
                }
                if (files.size() == 0) {
                    makeToast("No .csv file found in " + Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS));
                } else {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    final ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getApplicationContext(), R.layout.list_item_fix, files);
                    builder.setSingleChoiceItems(arrayAdapter, 0, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            try {
                                dataProvider.enableFileProviding(new FileReader(new File(Environment
                                        .getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + "/" + files.get(which))));
                            } catch (FileNotFoundException e) {
                                makeToast("Could not access file");
                            }
                            dialog.dismiss();
                        }
                    });
                    AlertDialog alert = builder.create();
                    alert.show();
                }
            }
        } else if (Environment.getExternalStorageState().equals(Environment.MEDIA_UNMOUNTED)) {
            makeToast("SD card not accessible");
        } else {
            makeToast("Unknown SD card state");
        }
    }

    /**
     * Start new bluetooth connection.
     * <p>
     * If bluetooth is not enabled, the user will be asked to do so. Shows a
     * list of available bluetooth devices.
     */
    private void initBluetoothCon() {
        if (!bluetoothAdapter.isEnabled() && bluetoothAdapter.getState() != BluetoothAdapter.STATE_TURNING_ON)
            askToEnableBT();
        else {

            // set up new connection now - start by choosing the remote device
            // (next: onActivityResult())
            Intent chooseDeviceIntent = new Intent(getApplicationContext(), DeviceListActivity.class);
            startActivityForResult(chooseDeviceIntent, INTENT_REQUEST_CHOOSE_DEVICE);

        }
    }

    @Override
    public void onBluetoothConnectionStateChanged(int state) {
        String text = "";
        if (state == BluetoothConnectionService.STATE_CONNECTED)
            text = "Connected.";
        else if (state == BluetoothConnectionService.STATE_CONNECTING)
            text = "Connecting...";
        else if (state == BluetoothConnectionService.STATE_NONE) text = "No connection.";
        makeToast(text);
    }

    @Override
    public void onBluetoothConnectionConnected(String name, String address) {
        makeToast("Connected to: " + name + " (" + address + ")");
    }

    @Override
    public void onBluetoothConnectionFailure(int whatFailed) {
        String text = "";
        if (whatFailed == BluetoothConnectionService.FAILURE_CONNECTION_LOST)
            text = "Bluetooth connection lost";
        else if (whatFailed == BluetoothConnectionService.FAILURE_WRITE_FAILED)
            text = "Bluetooth write failed";
        else if (whatFailed == BluetoothConnectionService.FAILURE_CONNECTING_FAILED) text = "Bluetooth connecting failed";
        makeToast(text);
    }

    private String parseMotionStatus(){
        int status;
        if (motionStata.size() == 0) status = -1;
        else status =  motionStata.get(Math.min(Math.max(motionStata.size()-1, 0), 2));

        // Anti random interference and transition smoothing
        if (motionStata.size() == 4) {
            int buf = motionStata.get(0);
            boolean isNoise = (motionStata.get(1) == buf) && (motionStata.get(3) == buf) && (status != buf);
            if (isNoise) {
                status = buf;
                /*
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tValue.setTextColor(Color.RED);
                    }
                });*/
            } else {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tValue.setTextColor(Color.BLACK);
                    }
                });

                // transition smoothing

                int[] motionStataArray = arrayList2Array(motionStata, true);
                // idle to jump
                if (Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_3TO2_1) ||
                        Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_3TO2_2) ||
                        Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_3TO2_3)) status = 2;
                // jump to idle
                else if (Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_2TO3_1) ||
                        Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_2TO3_2) ||
                        Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_2TO3_3)) status = 3;
                // walk to jump
                else if (Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_0TO2)) status = 2;
                // jump to walk
                else if (Arrays.equals(motionStataArray, TRANSITION_SMOOTHING_2TO0)) status = 0;

                // TODO jumping stabilization (TOO STRICT)
                else if (Arrays.equals(motionStataArray, JUMPING_SMOOTHING_1) ||
                        Arrays.equals(motionStataArray, JUMPING_SMOOTHING_2)) status = 2;
                else if (motionStataArray[0] == 2 && motionStataArray[3] == 2) status = 2;
            }
        }

        // log
        motionStataProtocol.add(status);
        if (motionStataProtocol.size() > 128) motionStataProtocol.remove(0);

        // refresh display
        switch (status){
            case 0:
                return "Walking";
            case 1:
                return "Running";
            case 2:
                return "Jumping";
            case 3:
                return "Idle";
            case 4:
                return "Standing";
            case -1:
                return "StandBy";
            default:
                return "Error lol";
        }
    }

    private String parseMotionStatus(int status){
        switch (status){
            case 0:
                return "Walking";
            case 1:
                return "Running";
            case 2:
                return "Jumping";
            case 3:
                return "Idle";
            case 4:
                return "Standing";
            case -1:
                return "StandBy";
            default:
                return "Error lol";
        }
    }

    public static <T> T mostCommon(List<T> list, T except) {
        Map<T, Integer> map = new HashMap<>();

        for (T t : list) {
            if (t.equals(except)) continue;
            Integer val = map.get(t);
            map.put(t, val == null ? 1 : val + 1);
        }

        Map.Entry<T, Integer> max = null;

        for (Map.Entry<T, Integer> e : map.entrySet()) {
            if (max == null || e.getValue() > max.getValue())
                max = e;
        }

        return max.getKey();
    }

    private double[] arrayList2Array(ArrayList<Double> al) {
        double[] a = new double[al.size()];
        for (int i = 0; i < al.size(); i++) {
            a[i] = al.get(i);
        }
        return a;
    }

    private int[] arrayList2Array(ArrayList<Integer> al, boolean dummy) {
        int[] a = new int[al.size()];
        for (int i = 0; i < al.size(); i++) {
            a[i] = al.get(i);
        }
        return a;
    }

    // starts a thread for motion recognition
    private void getMotionStatus(){
        if (!mrIsBusy) {
            mrIsBusy = true;
            new MotionStatusGetter().executeOnExecutor(AsyncTask.SERIAL_EXECUTOR, null, null, null);
        }
    }

    private double[] gain(double[] data) {
        double[] a = new double[data.length];
        for (int i = 0; i < data.length; i++) {
            a[i] = 23729.8 * data[i];
        }
        return a;
    }

    private class MotionStatusGetter extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params){
            //double[] xSequence = gain(arrayList2Array(accX));
            //double[] ySequence = gain(arrayList2Array(accY));
            //double[] zSequence = gain(arrayList2Array(accZ));
            double[] xSequence = arrayList2Array(accX);
            double[] ySequence = arrayList2Array(accY);
            double[] zSequence = arrayList2Array(accZ);

            // calculate feature values
            final double[] featureArray = featureBot.getFeatures(xSequence, ySequence, zSequence, useFullFeat);

            // update gui
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (!useFullFeat) {
                        maxSVMValue.setText(String.valueOf(featureArray[0]));
                        maxXValue.setText(String.valueOf(featureArray[1]));
                        minXValue.setText(String.valueOf(featureArray[2]));
                        stdSVMValue.setText(String.valueOf(featureArray[3]));
                        stdXValue.setText(String.valueOf(featureArray[4]));
                        meanPeakValue.setText(String.valueOf(featureArray[5]));
                    } else {
                        maxSVMValue.setText(String.valueOf(Math.ceil(featureArray[4])));
                        maxXValue.setText(String.valueOf(Math.ceil(featureArray[5])));
                        minXValue.setText(String.valueOf(Math.ceil(featureArray[9])));
                        stdSVMValue.setText(String.valueOf(Math.ceil(featureArray[12])));
                        stdXValue.setText(String.valueOf(Math.ceil(featureArray[13])));
                        meanPeakValue.setText(String.valueOf(Math.ceil(featureArray[16])));
                        //meanPeakValue.setText(String.valueOf(featureArray[3]));
                    }
                }
            });

            float[] floatFeatureArray = new float[featureArray.length];
            //makeToast(Arrays.toString(featureArray));
            for (int i = 0; i < featureArray.length; i++) {
                floatFeatureArray[i] = (float) featureArray[i];
            }
            try {
                // need purification
                //else if (featureArray[3] < 300 && featureArray[3] > 150) motionStatus = 1;
                //else if (featureArray[3] < 150 && featureArray[3] > 45) motionStatus = 0;
                //else if (featureArray[3] > 300) motionStatus = 2;
                // end
                if (!useFullFeat) {
                    if (featureArray[3] < 45.0) motionStatus = 3;
                    else if (featureArray[5] > 1000.0) motionStatus = 2;
                    else motionStatus = motionClassifier.recognizeMotion(floatFeatureArray);
                }
                else if (useFullFeat) {
                    if (featureArray[12] < 45.0) motionStatus = 3;
                    else if (featureArray[16] > 1000.0) motionStatus = 2;
                    else motionStatus = fullFeatureMotionClassifier.recognizeMotion(floatFeatureArray);
                }

                // extra jumping stabilization
                if (motionStatus != 2 && lastState == 2 && featureArray[featureArray.length-1] > 800) {
                    lastState = motionStatus;
                    motionStatus = 2;
                } else {
                    lastState = motionStatus;
                }

                // for general state stabilization
                motionStata.add(motionStatus);
                if (motionStata.size() > 4) motionStata.remove(0);

                // step counting
                if (isCountingSteps) {
                    if (isRealTimeCounting && tickTock) {
                        stepCount += featureBot.getSteps(xSequence, ySequence, zSequence, motionStatus);
                        calCount = 66 * stepCount / 1000;
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                stepsTextView.setText(String.valueOf(stepCount));
                                calV.setText(String.valueOf(calCount));
                            }
                        });
                    } else if (!isRealTimeCounting && tickTock) {
                        countX.addAll(accX);
                        countY.addAll(accY);
                        countZ.addAll(accZ);
                    }
                    if (countX.size() > 2559) {
                        double[] x = arrayList2Array(countX);
                        double[] y = arrayList2Array(countY);
                        double[] z = arrayList2Array(countZ);
                        countX.clear();
                        countY.clear();
                        countZ.clear();

                        stepCount += featureBot.getSteps(x, y, z, 10);
                        calCount = 66 * stepCount / 1000;
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                calV.setText(String.valueOf(calCount));
                                stepsTextView.setText(String.valueOf(stepCount));
                            }
                        });
                    }
                }


                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        switch (motionStata.size()) {
                            case 1:
                                tValue.setText(String.valueOf(parseMotionStatus(motionStata.get(0))));
                                break;
                            case 2:
                                tminus1Value.setText(String.valueOf(parseMotionStatus(motionStata.get(0))));
                                tValue.setText(String.valueOf(parseMotionStatus(motionStata.get(1))));
                                break;
                            case 3:
                                tminus2Value.setText(String.valueOf(parseMotionStatus(motionStata.get(0))));
                                tminus1Value.setText(String.valueOf(parseMotionStatus(motionStata.get(1))));
                                tValue.setText(String.valueOf(parseMotionStatus(motionStata.get(2))));
                                break;
                            case 4:
                                tminus2Value.setText(String.valueOf(parseMotionStatus(motionStata.get(0))));
                                tminus1Value.setText(String.valueOf(parseMotionStatus(motionStata.get(1))));
                                tValue.setText(String.valueOf(parseMotionStatus(motionStata.get(2))));
                                tplus1Value.setText(String.valueOf(parseMotionStatus(motionStata.get(3))));
                                break;
                        }
                    }
                });

            } catch (Exception e) {
                makeToast(e.getMessage());
            }

            mrIsBusy = false;
            return null;
        }
    }

    public static double round(double value, int places) {
        boolean isMinus = (value < 0);
        if (places < 0) throw new IllegalArgumentException();

        BigDecimal bd = new BigDecimal(value);
        bd = bd.setScale(places, RoundingMode.HALF_UP);
        if (isMinus) return 0 - bd.doubleValue();
        return bd.doubleValue();
    }

    /**
     * starts a thread that calculates the FFT spectrum

    private void getFrequencies() {
        if (!fftIsBusy) {
            fftIsBusy = true;
            new FrequencyGetter().executeOnExecutor(AsyncTask.SERIAL_EXECUTOR, null, null, null);
        }
    }
     */

    /**
     * This gets called in a separate thread and uses the JNI to call simulink
     * functions that Fast-Fourier-Transform the acceleration input.

    private class FrequencyGetter extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params) {

            for (int i = 0; i < PLOT_HISTORY_SIZE; i++)
                setModelInput(accX.get(i), i);
            stepModel();
            for (int i = 0; i < freqSeriesX.size(); i++)
                freqSeriesX.setY(Double.valueOf(getModelOutput(i)), i);

            for (int i = 0; i < PLOT_HISTORY_SIZE; i++)
                setModelInput(accY.get(i), i);
            stepModel();
            for (int i = 0; i < freqSeriesY.size(); i++)
                freqSeriesY.setY(Double.valueOf(getModelOutput(i)), i);

            for (int i = 0; i < PLOT_HISTORY_SIZE; i++)
                setModelInput(accZ.get(i), i);
            stepModel();
            for (int i = 0; i < freqSeriesZ.size(); i++)
                freqSeriesZ.setY(Double.valueOf(getModelOutput(i)), i);

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    freqPlot.redraw();
                }
            });
            fftIsBusy = false;

            return null;
        }
    }
     */

    /** display toast message. */
    private void makeToast(String text) {
        runOnUiThread(new ToastMaker(text));
    }

    /**
     * used by {@link MainActivity#makeToast(String)} to display a toast. Must
     * be run on the UI thread
     */
    private class ToastMaker implements Runnable {
        private String text;

        public ToastMaker(String text) {
            this.text = text;
        }

        @Override
        public void run() {
            Toast.makeText(getApplicationContext(), text, Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> arg0) {
    }

    @Override
    public void onBluetoothConnectionReceive(byte[] buffer, int numberOfBytesInBuffer) {
    }

    @Override
    public void onBluetoothConnectionWrite(byte[] buffer) {
    }
}
