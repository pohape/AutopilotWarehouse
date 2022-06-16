package com.droiduino.bluetoothconn;

import android.annotation.SuppressLint;
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
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

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

    @SuppressWarnings("unused")
    private final static int BT_COMMAND_WHEELS_LEFT_BACK_PRESSED = 25;
    @SuppressWarnings("unused")
    private final static int BT_COMMAND_WHEELS_LEFT_BACK_RELEASED = 26;

    @SuppressWarnings("unused")
    private final static int BT_COMMAND_WHEELS_RIGHT_BACK_PRESSED = 27;
    @SuppressWarnings("unused")
    private final static int BT_COMMAND_WHEELS_RIGHT_BACK_RELEASED = 28;
    private final static int BT_COMMAND_TURN_AROUND = 29;

    private final static int BT_COMMAND_SET_MODE_MANUAL = 101;
    private final static int BT_COMMAND_SET_MODE_FOLLOW_LINE = 102;
    private final static int BT_COMMAND_SET_MODE_TAKE_PACKAGE = 104;
    private final static int BT_COMMAND_SET_MODE_LEAVE_PACKAGE = 105;

    private final static int MODE_MANUAL = 1;
    private final static int MODE_FOLLOW_LINE = 2;
    private final static int MODE_AROUND_OBSTACLE = 3;
    private final static int MODE_TAKE_PACKAGE = 4;
    private final static int MODE_LEAVE_PACKAGE = 5;

    private final static int SWITCH_MODE_REASON_LINE_LOST = 1;
    private final static int SWITCH_MODE_REASON_LINE_ENDED = 2;
    private final static int SWITCH_MODE_REASON_FOUND_LINE = 3;
    private final static int SWITCH_MODE_REASON_FOUND_OBSTACLE = 4;
    private final static int SWITCH_MODE_REASON_PACKAGE_NOT_FOUND = 5;
    private final static int SWITCH_MODE_REASON_PACKAGE_LOST = 6;
    private final static int SWITCH_MODE_REASON_HOLD_PACKAGE = 7;
    @SuppressWarnings("unused")
    private final static int SWITCH_MODE_REASON_BLUETOOTH_COMMAND = 8;
    private final static int SWITCH_MODE_REASON_RELEASED_PACKAGE = 9;

    private int currentMode = MODE_MANUAL;
    private boolean armScreen = true;

    protected Toolbar toolbar;
    protected LinearLayout mainLayout;
    protected TextView warningText;

    protected Button buttonSwitchToFollowLine;
    protected Button buttonSwitchToTakePackage;
    protected Button buttonSwitchToLeavePackage;
    protected Button buttonSwitchToManual;

    protected Button buttonTurnAround;
    protected Button buttonForward;
    protected Button buttonBack;
    protected Button buttonLeft;
    protected Button buttonRight;
    protected Button buttonUp;
    protected Button buttonDown;
    protected Button buttonOpen;
    protected Button buttonClose;

    protected Button buttonConnect;
    protected ProgressBar progressBarConnecting;

    protected long lastBtReceive = 0;
    protected Runnable checkBtConnection;

    protected void disableAllJoystickButtons() {
        buttonUp.setEnabled(false);
        buttonDown.setEnabled(false);
        buttonOpen.setEnabled(false);
        buttonClose.setEnabled(false);

        buttonTurnAround.setEnabled(false);
        buttonForward.setEnabled(false);
        buttonLeft.setEnabled(false);
        buttonRight.setEnabled(false);
        buttonBack.setEnabled(false);
    }

    protected void setVisibilityForModeButtons(boolean manual, boolean followLine, boolean takePackage, boolean leavePackage) {
        buttonSwitchToManual.setEnabled(manual);
        buttonSwitchToFollowLine.setEnabled(followLine);
        buttonSwitchToTakePackage.setEnabled(takePackage);
        buttonSwitchToLeavePackage.setEnabled(leavePackage);
    }

    protected void setButtonsVisibility(boolean manual, boolean followLine, boolean takePackage, boolean leavePackage) {
        if (manual) {
            buttonSwitchToManual.setText("Ручной режим");
        } else {
            String buttonSwitchToManualText = "Управлять " + (armScreen ? "колесами" : "рукой");
            buttonSwitchToManual.setText(buttonSwitchToManualText);
        }

        setVisibilityForModeButtons(true, followLine, takePackage, leavePackage);

        mainLayout.setVisibility(View.VISIBLE);
        warningText.setVisibility(View.GONE);
    }

    protected void switchScreenToManual() {
        currentMode = MODE_MANUAL;
        lastBtReceive = 0;

        toolbar.setTitle("Текущий режим: ручное управление " + (armScreen ? "рукой" : "колесами"));
        setButtonsVisibility(false, true, true, true);

        buttonUp.setEnabled(armScreen);
        buttonDown.setEnabled(armScreen);
        buttonOpen.setEnabled(armScreen);
        buttonClose.setEnabled(armScreen);

        buttonTurnAround.setEnabled(true);
        buttonForward.setEnabled(true);
        buttonLeft.setEnabled(true);
        buttonRight.setEnabled(true);
        buttonBack.setEnabled(true);
    }

    protected void switchScreenToFollowLine() {
        currentMode = MODE_FOLLOW_LINE;
        toolbar.setTitle("Текущий режим: следую за линией");

        setButtonsVisibility(true, false, false, false);
        disableAllJoystickButtons();
    }

    protected void switchScreenToObstacle() {
        currentMode = MODE_AROUND_OBSTACLE;
        toolbar.setTitle("Текущий режим: объезжаю препятствие");

        setButtonsVisibility(true, false, false, false);
        disableAllJoystickButtons();
    }

    protected void switchScreenToTakePackage() {
        currentMode = MODE_TAKE_PACKAGE;
        toolbar.setTitle("Текущий режим: беру коробку");

        setButtonsVisibility(true, false, false, false);
        disableAllJoystickButtons();
    }

    protected void switchScreenLeavePackage() {
        currentMode = MODE_TAKE_PACKAGE;
        toolbar.setTitle("Текущий режим: выгружаю коробку");

        setButtonsVisibility(true, false, false, false);
        disableAllJoystickButtons();
    }

    protected void showMessage(String text) {
        mainLayout.setVisibility(View.GONE);
        warningText.setVisibility(View.VISIBLE);
        warningText.setText(text);
    }

    protected void showWarning(String text) {
        showMessage(text);

        handler.postDelayed(() -> {
            mainLayout.setVisibility(View.VISIBLE);
            warningText.setVisibility(View.GONE);
            warningText.setText("");
        }, 1_000);
    }

    protected void showConnected() {
        toolbar.setSubtitle("Подключено к " + deviceName);
        progressBarConnecting.setVisibility(View.GONE);

        buttonConnect.setEnabled(true);
        buttonConnect.setText("Переподключиться");

        switchScreenToManual();
    }

    protected void showCouldNotConnect() {
        showMessage("Не удаётся подключиться!\nПопробуйте еще раз или перезапустите!");
        toolbar.setSubtitle("Не удаётся подключиться");
        progressBarConnecting.setVisibility(View.GONE);

        buttonConnect.setEnabled(true);
        buttonConnect.setText("Подключиться");

        disableAllJoystickButtons();
        setVisibilityForModeButtons(false, false, false, false);
    }

    protected void showLostConnection() {
        showMessage("Потеряно соединение с машиной!\nПопробуйте еще раз или перезапустите!");
        toolbar.setSubtitle("Потеряно соединение с машиной");
        progressBarConnecting.setVisibility(View.GONE);

        buttonConnect.setEnabled(true);
        buttonConnect.setText("Подключиться");

        disableAllJoystickButtons();
        setVisibilityForModeButtons(false, false, false, false);
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // UI Initialization
        buttonConnect = findViewById(R.id.buttonConnect);
        progressBarConnecting = findViewById(R.id.progressBarConnecting);
        toolbar = findViewById(R.id.toolbar);
        mainLayout = findViewById(R.id.mainLayout);
        warningText = findViewById(R.id.warningText);

        // buttons
        buttonSwitchToFollowLine = findViewById(R.id.followLineMode);
        buttonSwitchToFollowLine.setEnabled(false);

        buttonSwitchToTakePackage = findViewById(R.id.takePackageMode);
        buttonSwitchToTakePackage.setEnabled(false);

        buttonSwitchToLeavePackage = findViewById(R.id.leavePackageMode);
        buttonSwitchToLeavePackage.setEnabled(false);

        buttonSwitchToManual = findViewById(R.id.manualMode);
        buttonSwitchToManual.setEnabled(false);

        buttonForward = findViewById(R.id.buttonForward);
        buttonForward.setEnabled(false);

        buttonTurnAround = findViewById(R.id.buttonTurnAround);
        buttonTurnAround.setEnabled(false);

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

        showMessage("Добро пожаловать!\nПодключите машину по Bluetooth.");

        // If a bluetooth device has been selected from SelectDeviceActivity
        deviceName = getIntent().getStringExtra("deviceName");
        if (deviceName != null) {
            // Get the device address to make BT Connection
            String deviceAddress = getIntent().getStringExtra("deviceAddress");
            // Show progress and connection status
            toolbar.setSubtitle("Пытаюсь соединиться с " + deviceName + "...");
            warningText.setVisibility(View.GONE);
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
                                showConnected();
                                break;
                            case -1:
                                showCouldNotConnect();

                                break;
                        }
                        break;

                    case MESSAGE_READ:
                        lastBtReceive = System.currentTimeMillis();

                        char[] arduinoMsg = msg.obj.toString().trim().toCharArray(); // Read message from Arduino
                        int newMode = Integer.valueOf(String.valueOf(arduinoMsg[0]));

                        if (newMode != 0) {
                            Log.i("Bluetooth_app", String.valueOf(arduinoMsg[0]));
                            int reason = Integer.valueOf(String.valueOf(arduinoMsg[1]));

                            switch (newMode) {
                                case MODE_MANUAL:
                                    lastBtReceive = 0;
                                    switchScreenToManual();
                                    break;
                                case MODE_FOLLOW_LINE:
                                    switchScreenToFollowLine();
                                    break;
                                case MODE_AROUND_OBSTACLE:
                                    switchScreenToObstacle();
                                    break;
                                case MODE_TAKE_PACKAGE:
                                    switchScreenToTakePackage();
                                    break;
                                case MODE_LEAVE_PACKAGE:
                                    switchScreenLeavePackage();
                                    break;
                            }

                            if (reason == SWITCH_MODE_REASON_FOUND_LINE) {
                                showWarning("Линия найдена.\nСледую по линии.");
                            } else if (reason == SWITCH_MODE_REASON_FOUND_OBSTACLE) {
                                showWarning("Вижу препятствие.\nПытаюсь объехать.");
                            } else if (reason == SWITCH_MODE_REASON_LINE_ENDED) {
                                showWarning("Линия закончилась!\nЧто будем делать дальше?");
                            } else if (reason == SWITCH_MODE_REASON_LINE_LOST) {
                                showWarning("Потерял линию!\nОператор, помоги!");
                            } else if (reason == SWITCH_MODE_REASON_PACKAGE_LOST) {
                                showWarning("Потерял коробку.\nОператор, помоги!");
                            } else if (reason == SWITCH_MODE_REASON_PACKAGE_NOT_FOUND) {
                                showWarning("Не могу найти коробку.\nОператор, помоги!");
                            } else if (reason == SWITCH_MODE_REASON_HOLD_PACKAGE) {
                                showWarning("Держу коробку.\nЧто будем делать дальше?");
                            } else if (reason == SWITCH_MODE_REASON_RELEASED_PACKAGE) {
                                showWarning("Выгрузили коробку.\nЧто будем делать дальше?");
                            }
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

        checkBtConnection = new Runnable() {
            @Override
            public void run() {
                boolean mObstacle = currentMode == MODE_AROUND_OBSTACLE;
                boolean mFollowLine = currentMode == MODE_FOLLOW_LINE;
                boolean mTakePackage = currentMode == MODE_TAKE_PACKAGE;
                boolean lastBtReceiveExpired = (lastBtReceive + 20_000) > System.currentTimeMillis();

                if (!mObstacle && !mFollowLine && !mTakePackage && lastBtReceive != 0 && !lastBtReceiveExpired) {
                    showLostConnection();
                } else if (deviceName != null) {
                    showConnected();
                }

                handler.postDelayed(this, 15_000);
            }
        };

        handler.postDelayed(checkBtConnection, 15_000);

        // buttons on click
        buttonSwitchToTakePackage.setOnClickListener(v -> connectedThread.write(BT_COMMAND_SET_MODE_TAKE_PACKAGE));

        buttonSwitchToLeavePackage.setOnClickListener(v -> connectedThread.write(BT_COMMAND_SET_MODE_LEAVE_PACKAGE));

        buttonSwitchToFollowLine.setOnClickListener(v -> connectedThread.write(BT_COMMAND_SET_MODE_FOLLOW_LINE));

        buttonSwitchToManual.setOnClickListener(v -> {
            if (currentMode == MODE_MANUAL) {
                armScreen = !armScreen;
                switchScreenToManual();
            } else {
                connectedThread.write(BT_COMMAND_SET_MODE_MANUAL);
            }
        });

        buttonTurnAround.setOnClickListener(v -> connectedThread.write(BT_COMMAND_TURN_AROUND));

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

        CreateConnectThread(BluetoothAdapter bluetoothAdapter, String address) {
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
        void cancel() {
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

        ConnectedThread(BluetoothSocket socket) {
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
        @SuppressWarnings("unused")
        public void write(String input) {
            byte[] bytes = input.getBytes(); //converts entered String into bytes
            try {
                mmOutStream.write(bytes);
                mmOutStream.flush();
            } catch (IOException e) {
                Log.e("Send Error", "Unable to send message", e);
            }
        }

        void write(int input) {
            try {
                mmOutStream.write(input);
                mmOutStream.flush();
            } catch (IOException e) {
                Log.e("Send Error", "Unable to send message", e);
            }
        }

        /* Call this from the main activity to shutdown the connection */
        @SuppressWarnings("unused")
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
