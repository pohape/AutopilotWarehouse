package com.droiduino.bluetoothconn;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import static android.content.ContentValues.TAG;

public class MainActivity extends AppCompatActivity {

    private String deviceName = null;
    public static Handler handler;
    public static BluetoothSocket mmSocket;
    public static ConnectedThread connectedThread;
    public static CreateConnectThread createConnectThread;

    private final static int CONNECTING_STATUS = 1; // used in bluetooth handler to identify message status
    private final static int MESSAGE_READ = 2; // used in bluetooth handler to identify message update

    private final static int BT_COMMAND_ARM_FORWARD_PRESSED = 1;
    private final static int BT_COMMAND_ARM_FORWARD_RELEASED = 2;

    private final static int BT_COMMAND_ARM_BACK_PRESSED = 3;
    private final static int BT_COMMAND_ARM_BACK_RELEASED = 4;

    private final static int BT_COMMAND_ARM_LEFT_PRESSED = 5;
    private final static int BT_COMMAND_ARM_LEFT_RELEASED = 6;

    private final static int BT_COMMAND_ARM_RIGHT_PRESSED = 7;
    private final static int BT_COMMAND_ARM_RIGHT_RELEASED = 8;

    private final static int BT_COMMAND_ARM_UP_PRESSED = 9;
    private final static int BT_COMMAND_ARM_UP_RELEASED = 10;

    private final static int BT_COMMAND_ARM_DOWN_PRESSED = 11;
    private final static int BT_COMMAND_ARM_DOWN_RELEASED = 12;

    private final static int BT_COMMAND_ARM_OPEN_CLAW_PRESSED = 13;
    private final static int BT_COMMAND_ARM_OPEN_CLAW_RELEASED = 14;

    private final static int BT_COMMAND_ARM_CLOSE_CLAW_PRESSED = 15;
    private final static int BT_COMMAND_ARM_CLOSE_CLAW_RELEASED = 16;

    private final static int BT_COMMAND_WHEELS_FORWARD_PRESSED = 17;
    private final static int BT_COMMAND_WHEELS_FORWARD_RELEASED = 18;

    private final static int BT_COMMAND_WHEELS_LEFT_FORWARD_PRESSED = 19;
    private final static int BT_COMMAND_WHEELS_LEFT_FORWARD_RELEASED = 20;

    private final static int BT_COMMAND_WHEELS_RIGHT_FORWARD_PRESSED = 21;
    private final static int BT_COMMAND_WHEELS_RIGHT_FORWARD_RELEASED = 22;

    private final static int BT_COMMAND_WHEELS_BACK_PRESSED = 23;
    private final static int BT_COMMAND_WHEELS_BACK_RELEASED = 24;

    private final static int BT_COMMAND_WHEELS_LEFT_BACK_PRESSED = 25;
    private final static int BT_COMMAND_WHEELS_LEFT_BACK_RELEASED = 26;

    private final static int BT_COMMAND_WHEELS_RIGHT_BACK_PRESSED = 27;
    private final static int BT_COMMAND_WHEELS_RIGHT_BACK_RELEASED = 28;

    private final static int BT_COMMAND_SWITCH_MODE = 29;

    private int mode = 1;
    private boolean armScreen = true;

    protected Toolbar toolbar;

    protected Button buttonSwitchToDrivingOrToArm;
    protected Button buttonSwitchToFollowLineOrManual;
    protected Button buttonForward;
    protected Button buttonBack;
    protected Button buttonLeft;
    protected Button buttonRight;
    protected Button buttonUp;
    protected Button buttonDown;
    protected Button buttonOpen;
    protected Button buttonClose;

    protected void switchScreenToManual() {
        mode = 1;
        toolbar.setTitle("Current mode: manual " + (armScreen ? "ARM" : "DRIVING"));
        buttonSwitchToFollowLineOrManual.setText("Switch to follow line mode");
        buttonSwitchToDrivingOrToArm.setText("Switch to " + (armScreen ? "DRIVING" : "ARM control"));

        buttonSwitchToDrivingOrToArm.setEnabled(true);
        buttonSwitchToFollowLineOrManual.setEnabled(true);

        buttonUp.setEnabled(armScreen);
        buttonDown.setEnabled(armScreen);
        buttonOpen.setEnabled(armScreen);
        buttonClose.setEnabled(armScreen);

        buttonForward.setEnabled(true);
        buttonLeft.setEnabled(true);
        buttonRight.setEnabled(true);
        buttonBack.setEnabled(true);


        Toast.makeText(MainActivity.this, "Switched to MANUAL mode", Toast.LENGTH_SHORT).show();
    }

    protected void switchScreenToFollowLine() {
        mode = 2;
        toolbar.setTitle("Current mode: following the line");
        buttonSwitchToFollowLineOrManual.setText("Switch to manual mode");

        buttonSwitchToDrivingOrToArm.setEnabled(true);
        buttonSwitchToFollowLineOrManual.setEnabled(true);

        buttonUp.setEnabled(false);
        buttonDown.setEnabled(false);
        buttonOpen.setEnabled(false);
        buttonClose.setEnabled(false);

        buttonForward.setEnabled(false);
        buttonLeft.setEnabled(false);
        buttonRight.setEnabled(false);
        buttonBack.setEnabled(false);
        Toast.makeText(MainActivity.this, "Switched to FOLLOW LINE mode", Toast.LENGTH_SHORT).show();
    }

    protected void switchScreenToObstacle() {
        mode = 3;
        toolbar.setTitle("Current mode: going around an obstacle");
        buttonSwitchToFollowLineOrManual.setText("Switch to manual mode");

        buttonSwitchToDrivingOrToArm.setEnabled(true);
        buttonSwitchToFollowLineOrManual.setEnabled(true);

        buttonUp.setEnabled(false);
        buttonDown.setEnabled(false);
        buttonOpen.setEnabled(false);
        buttonClose.setEnabled(false);

        buttonForward.setEnabled(false);
        buttonLeft.setEnabled(false);
        buttonRight.setEnabled(false);
        buttonBack.setEnabled(false);
        Toast.makeText(MainActivity.this, "Switched to OBSTACLE mode", Toast.LENGTH_SHORT).show();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        toolbar = findViewById(R.id.toolbar);

        // buttons
        buttonSwitchToDrivingOrToArm = findViewById(R.id.switchScreen);
        buttonSwitchToDrivingOrToArm.setEnabled(false);

        buttonSwitchToFollowLineOrManual = findViewById(R.id.switchMode);
        buttonSwitchToFollowLineOrManual.setEnabled(false);

        buttonForward = findViewById(R.id.buttonForward);
        buttonForward.setEnabled(false);

        buttonBack = findViewById(R.id.buttonBack);
        buttonBack.setEnabled(false);

        buttonLeft = findViewById(R.id.buttonLeft);
        buttonLeft.setEnabled(false);

        buttonRight = findViewById(R.id.buttonRight);
        buttonRight.setEnabled(false);

        buttonUp = findViewById(R.id.buttonUp);
        buttonUp.setEnabled(false);

        buttonDown = findViewById(R.id.buttonDown);
        buttonDown.setEnabled(false);

        buttonOpen = findViewById(R.id.buttonOpen);
        buttonOpen.setEnabled(false);

        buttonClose = findViewById(R.id.buttonClose);
        buttonClose.setEnabled(false);

        // UI Initialization
        final Button buttonConnect = findViewById(R.id.buttonConnect);
        final Toolbar toolbar = findViewById(R.id.toolbar);
        final ProgressBar progressBarConnecting = findViewById(R.id.progressBarConnecting);
        final TextView textViewArduinoMessages = findViewById(R.id.textViewArduinoMessages);

        // If a bluetooth device has been selected from SelectDeviceActivity
        deviceName = getIntent().getStringExtra("deviceName");
        if (deviceName != null) {
            // Get the device address to make BT Connection
            String deviceAddress = getIntent().getStringExtra("deviceAddress");
            // Show progress and connection status
            toolbar.setSubtitle("Connecting to " + deviceName + "...");
            progressBarConnecting.setVisibility(View.VISIBLE);
            buttonConnect.setEnabled(false);

            /*
            This is the most important piece of code. When "deviceName" is found
            the code will call a new thread to create a bluetooth connection to the
            selected device (see the thread code below)
             */
            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            createConnectThread = new CreateConnectThread(bluetoothAdapter, deviceAddress);
            createConnectThread.start();
        }

        /*
        Second most important piece of Code. GUI Handler
         */
        handler = new Handler(Looper.getMainLooper()) {
            @Override
            public void handleMessage(@NonNull Message msg) {
                switch (msg.what) {
                    case CONNECTING_STATUS:
                        switch (msg.arg1) {
                            case 1:
                                toolbar.setSubtitle("Connected to " + deviceName);
                                progressBarConnecting.setVisibility(View.GONE);

                                buttonConnect.setEnabled(true);
                                switchScreenToManual();

                                break;
                            case -1:
                                toolbar.setSubtitle("Device fails to connect");
                                progressBarConnecting.setVisibility(View.GONE);

                                buttonForward.setEnabled(false);
                                buttonBack.setEnabled(false);

                                buttonRight.setEnabled(false);
                                buttonLeft.setEnabled(false);

                                buttonUp.setEnabled(false);
                                buttonDown.setEnabled(false);

                                buttonOpen.setEnabled(false);
                                buttonClose.setEnabled(false);

                                buttonSwitchToDrivingOrToArm.setEnabled(false);
                                buttonSwitchToFollowLineOrManual.setEnabled(false);

                                break;
                        }
                        break;

                    case MESSAGE_READ:
                        String arduinoMsg = msg.obj.toString().trim(); // Read message from Arduino

                        if (arduinoMsg.equals("1")) {
                            switchScreenToManual();
                        } else if (arduinoMsg.equals("2")) {
                            switchScreenToFollowLine();
                        } else if (arduinoMsg.equals("3")) {
                            switchScreenToObstacle();
                        }

                        break;
                }
            }
        };

        // Select Bluetooth Device
        buttonConnect.setOnClickListener(view -> {
            // Move to adapter list
            Intent intent = new Intent(MainActivity.this, SelectDeviceActivity.class);
            startActivity(intent);
        });

        // buttons on click
        buttonSwitchToDrivingOrToArm.setOnClickListener(v -> {
            armScreen = !armScreen;
            switchScreenToManual();
        });

        buttonSwitchToFollowLineOrManual.setOnClickListener(v -> {
            connectedThread.write(BT_COMMAND_SWITCH_MODE);

//            if (mode == 1) {
//                switchScreenToFollowLine();
//            } else if (mode == 2 || mode == 3) {
//                switchScreenToManual();
//            }
        });

        buttonForward.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_FORWARD_PRESSED : BT_COMMAND_WHEELS_FORWARD_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_FORWARD_RELEASED : BT_COMMAND_WHEELS_FORWARD_RELEASED);
            }

            return true;
        });

        buttonBack.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_BACK_PRESSED : BT_COMMAND_WHEELS_BACK_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_BACK_RELEASED : BT_COMMAND_WHEELS_BACK_RELEASED);
            }

            return true;
        });

        buttonDown.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(BT_COMMAND_ARM_DOWN_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(BT_COMMAND_ARM_DOWN_RELEASED);
            }

            return true;
        });

        buttonUp.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(BT_COMMAND_ARM_UP_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(BT_COMMAND_ARM_UP_RELEASED);
            }

            return true;
        });

        buttonLeft.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_LEFT_PRESSED : BT_COMMAND_WHEELS_LEFT_FORWARD_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_LEFT_RELEASED : BT_COMMAND_WHEELS_LEFT_FORWARD_RELEASED);
            }

            return true;
        });

        buttonRight.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_RIGHT_PRESSED : BT_COMMAND_WHEELS_RIGHT_FORWARD_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(armScreen ? BT_COMMAND_ARM_RIGHT_RELEASED : BT_COMMAND_WHEELS_RIGHT_FORWARD_RELEASED);
            }

            return true;
        });

        buttonOpen.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(BT_COMMAND_ARM_OPEN_CLAW_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(BT_COMMAND_ARM_OPEN_CLAW_RELEASED);
            }

            return true;
        });

        buttonClose.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                connectedThread.write(BT_COMMAND_ARM_CLOSE_CLAW_PRESSED);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                connectedThread.write(BT_COMMAND_ARM_CLOSE_CLAW_RELEASED);
            }

            return true;
        });
    }

    /* ============================ Thread to Create Bluetooth Connection =================================== */
    public static class CreateConnectThread extends Thread {

        public CreateConnectThread(BluetoothAdapter bluetoothAdapter, String address) {
            /*
            Use a temporary object that is later assigned to mmSocket
            because mmSocket is final.
             */
            BluetoothDevice bluetoothDevice = bluetoothAdapter.getRemoteDevice(address);
            BluetoothSocket tmp = null;
            UUID uuid = bluetoothDevice.getUuids()[0].getUuid();

            try {
                /*
                Get a BluetoothSocket to connect with the given BluetoothDevice.
                Due to Android device varieties,the method below may not work fo different devices.
                You should try using other methods i.e. :
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
                 */
                tmp = bluetoothDevice.createInsecureRfcommSocketToServiceRecord(uuid);

            } catch (IOException e) {
                Log.e(TAG, "Socket's create() method failed", e);
            }
            mmSocket = tmp;
        }

        public void run() {
            // Cancel discovery because it otherwise slows down the connection.
            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            bluetoothAdapter.cancelDiscovery();
            try {
                // Connect to the remote device through the socket. This call blocks
                // until it succeeds or throws an exception.
                mmSocket.connect();
                Log.e("Status", "Device connected");
                handler.obtainMessage(CONNECTING_STATUS, 1, -1).sendToTarget();
            } catch (IOException connectException) {
                // Unable to connect; close the socket and return.
                try {
                    mmSocket.close();
                    Log.e("Status", "Cannot connect to device");
                    handler.obtainMessage(CONNECTING_STATUS, -1, -1).sendToTarget();
                } catch (IOException closeException) {
                    Log.e(TAG, "Could not close the client socket", closeException);
                }
                return;
            }

            // The connection attempt succeeded. Perform work associated with
            // the connection in a separate thread.
            connectedThread = new ConnectedThread(mmSocket);
            connectedThread.start();
        }

        // Closes the client socket and causes the thread to finish.
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "Could not close the client socket", e);
            }
        }
    }

    /* =============================== Thread for Data Transfer =========================================== */
    public static class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException ignored) {
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes = 0; // bytes returned from read()
            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    /*
                    Read from the InputStream from Arduino until termination character is reached.
                    Then send the whole String message to GUI Handler.
                     */
                    buffer[bytes] = (byte) mmInStream.read();
                    String readMessage;
                    if (buffer[bytes] == '\n') {
                        readMessage = new String(buffer, 0, bytes);
                        Log.e("Arduino Message", readMessage);
                        handler.obtainMessage(MESSAGE_READ, readMessage).sendToTarget();
                        bytes = 0;
                    } else {
                        bytes++;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        public void write(String input) {
            byte[] bytes = input.getBytes(); //converts entered String into bytes
            try {
                mmOutStream.write(bytes);
                mmOutStream.flush();
            } catch (IOException e) {
                Log.e("Send Error", "Unable to send message", e);
            }
        }

        public void write(int input) {
            try {
                mmOutStream.write(input);
                mmOutStream.flush();
            } catch (IOException e) {
                Log.e("Send Error", "Unable to send message", e);
            }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException ignored) {
            }
        }
    }

    /* ============================ Terminate Connection at BackPress ====================== */
    @Override
    public void onBackPressed() {
        // Terminate Bluetooth Connection and close app
        if (createConnectThread != null) {
            createConnectThread.cancel();
        }
        Intent a = new Intent(Intent.ACTION_MAIN);
        a.addCategory(Intent.CATEGORY_HOME);
        a.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(a);
    }
}
