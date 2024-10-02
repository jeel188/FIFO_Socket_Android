//
// Created by Patel on 30-09-2024.
//
#include <iostream>
#include "Demo.h"

int main() {
    LOGE("Executable Running");
    std::cout << "CPP Running !!!!" << std::endl;

    FIFOServer server;         // Create an instance of the FIFOServer
    Request request{};         // Initialize a Request object
    Response response{};       // Initialize a Response object

    // Create FIFOs for request and response
    if (!server.Create(FIFO_REQUEST, FIFO_RESPONSE)) {
        LOGE("Server Cant Create");
        server.Close();
        return 0;  // Exit if the server can't create FIFOs
    }

    // Bind the server to the created FIFOs
    if (!server.Bind(FIFO_REQUEST, FIFO_RESPONSE)) {
        LOGE("Socket Bind Failed !!");
        std::cout << "Socket Bind Failed !!" << std::endl;
        server.Close();
        return 0;  // Exit if binding fails
    }

    LOGE("Socket Connected !!");
    std::cout << "Socket Connected !!" << std::endl;

    // Wait to receive a request
    while (server.receive((void *) &request, sizeof(request)) > 0) {
        LOGI("Request Received Success !!");

        LOGE("Request Received is: %s", request.app_name);
        LOGE("Request Received is: %s", request.pkg_name);
        LOGE("Request Received is: %d", request.code);
        // Prepare the response
        strcpy(response.Message, "Message From Jeel Jod");
        strcpy(response.developedBy, "LearnGG Team");
        response.isSuccess = true;

        // Check the request code; if it's 0, close the server
        if (request.code == 0) {
            LOGE("Closing Server...");
            std::cout << "Closing Server..." << std::endl;
            server.Close();
            return 0;  // Exit after closing the server
        }

        // Send the prepared response
        server.send((void *) &response, sizeof(response));
    }

    return 0;  // Exit main function
}
