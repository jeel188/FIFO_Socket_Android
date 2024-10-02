package com.learn.gg.fifo;

import java.io.DataOutputStream;
import java.io.IOException;

public class RootUtil {

    // Method to check if the device is rooted by attempting to run "su"
    public static boolean isDeviceRooted() {
        return executeCommandAsRoot("echo test") == 0;
    }

    // Method to execute a command as root
    public static int executeCommandAsRoot(String command) {
        Process process = null;
        DataOutputStream os = null;
        try {
            // Attempt to run the 'su' command
            process = Runtime.getRuntime().exec("su");
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes(command + "\n");
            os.writeBytes("exit\n");
            os.flush();

            // Wait for the command to finish and return the exit value
            process.waitFor();
            return process.exitValue();  // Return 0 for success, or other value for failure
        } catch (Exception e) {
            e.printStackTrace();
            return -1;  // Return -1 if there was an exception
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
                if (process != null) {
                    process.destroy();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
