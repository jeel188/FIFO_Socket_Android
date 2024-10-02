package com.learn.gg.fifo;

import android.annotation.SuppressLint;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;


import com.learn.gg.fifo.databinding.ActivityMainBinding;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("demo");
    }

    private static final String SOCKET_EXECUTABLE_32 = "executable";
    private static final String SOCKET_EXECUTABLE_64 = "executable64";
    private static boolean isCommandThreadRunning = false;
    private static boolean isSuccess = true;
    private String socket;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        boolean isRooted = RootUtil.isDeviceRooted();

        if (!isRooted) {
            showRootPermissionAlert();
            return;
        }

        ActivityMainBinding binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        boolean is32Bit = !(Integer.parseInt(Build.VERSION.RELEASE) >= 10);
        TextView tv = binding.sampleText;
        Button startBtn = binding.start;
        Button stopBtn = binding.stop;

        loadAssets(is32Bit);
        initializeServer(tv);

        startBtn.setOnClickListener(view -> {
            if (isSuccess) {
                tv.setText(stringFromJNI(1));
            } else {
                Toast.makeText(MainActivity.this, "Press Start Button Again", Toast.LENGTH_SHORT).show();
                isSuccess = isServerRunning(socket);
            }
        });

        stopBtn.setOnClickListener(view -> {
            isSuccess = false;
            isCommandThreadRunning = false;
            tv.setText(stringFromJNI(0));
        });
    }

    private void showRootPermissionAlert() {
        new AlertDialog.Builder(this)
                .setTitle("Root Permission Required")
                .setMessage("This app requires superuser (root) permission to function properly. Please grant root access when prompted.")
                .setNegativeButton("Ok", (dialogInterface, i) -> finish())
                .create()
                .show();
    }

    private void loadAssets(boolean is32Bit) {
        String socketName = is32Bit ? SOCKET_EXECUTABLE_32 : SOCKET_EXECUTABLE_64;
        String path = getFilesDir() + "/" + socketName;
        try (OutputStream myOutput = new FileOutputStream(path);
             InputStream myInput = getAssets().open(socketName)) {

            byte[] buffer = new byte[1024];
            int length;
            while ((length = myInput.read(buffer)) > 0) {
                myOutput.write(buffer, 0, length);
            }
            myOutput.flush();
            Runtime.getRuntime().exec("chmod 777 " + path);
        } catch (IOException e) {
            Log.e("JeelPatel", "Failed to open socket: " + e.getMessage());
            Toast.makeText(this, "Failed to load executable", Toast.LENGTH_SHORT).show();
        }
        socket = "su -c " + path;
    }

    private void initializeServer(TextView tv) {
        new Handler(Looper.getMainLooper()).post(() -> {
            isSuccess = isServerRunning(socket);
            while (!isSuccess) {
                Toast.makeText(this, "Initializing Server", Toast.LENGTH_LONG).show();
                isSuccess = isServerRunning(socket);
            }
            if (RootUtil.executeCommandAsRoot("setenforce 0") != -1) {
                Toast.makeText(this, "Success Initialization", Toast.LENGTH_LONG).show();
            }
        });
    }

    static boolean isServerRunning(String socket) {
        if (!isCommandThreadRunning) {
            isCommandThreadRunning = true;
            new Thread(() -> {
                try {
                    Thread.sleep(400);
                    RootUtil.executeCommandAsRoot(socket);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    isCommandThreadRunning = false;
                }
            }).start();
        }
        return isCommandThreadRunning;
    }


    public native String stringFromJNI(int code);
}
