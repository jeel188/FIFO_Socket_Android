#include "native-lib.h"

// Define a maximum number of attempts to receive a response
#define MAX_ATTEMPTS 5

// JNI function implementation
extern "C" JNIEXPORT jstring JNICALL
Java_com_learn_gg_fifo_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */,
                                                         jint code) {
    LOGI("Function Called !!!");

    FIFOClient client;        // Create an instance of FIFOClient
    Request request{};        // Initialize a Request object
    Response response{};      // Initialize a Response object


    strcpy(request.app_name ,"Demo App");
    strcpy(request.pkg_name ,"com.demo.app");
    request.code = int(code);

    // Create the FIFO client
    if (!client.Create()) {
        LOGE("Failed to create FIFO client");
        client.Close();
        return env->NewStringUTF("FIFO client creation failed");

    }

    // Connect to the server using FIFO
    if (!client.Connect(FIFO_REQUEST, FIFO_RESPONSE)) {
        LOGE("Client not connected");
        client.Close();
        return env->NewStringUTF("Client not connected"); // Return error message
    }

    LOGI("Client Successfully Connected With Server !!");

    if (!client.send((void *) &request, sizeof(request))) {
        LOGE("Failed to send request to server");
        client.Close();
        return env->NewStringUTF("Failed to send request");
    } else {
        LOGE("Data sent Successfully to server!!");
    }
    client.receive((void *)&response, sizeof(response));
    return env->NewStringUTF(response.Message); // Return error message
}
