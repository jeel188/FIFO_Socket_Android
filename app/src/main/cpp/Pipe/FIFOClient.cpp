#include "FIFOClient.h"
#include "Log.h"

FIFOClient::FIFOClient(){
    created = false;
    connected = false;
}

bool FIFOClient::Create() {
    created = true;
    return created;
}

bool FIFOClient::Connect(const char* req, const char* resp) {
	/* open FIFO */
	request_client_to_server = open(/*FIFO_REQUEST*/ req, O_WRONLY);
	if (request_client_to_server == -1) {
        LOGE("Error opening the request FIFO pipe for writing");
        LOGE("Error \"%s\" in Connect()\n", strerror(errno));
        return false;
    }
    response_server_to_client = open(/*FIFO_RESPONSE*/ resp, O_RDONLY);
	if (response_server_to_client == -1) {
        LOGE("Error opening the response FIFO pipe for reading");
        LOGE("Error \"%s\" in Connect()\n", strerror(errno));
        return false;
    }
	connected = true;
    return true;
}

int FIFOClient::sendData(void *inData, size_t size) {
    char *buffer = (char *) inData;
    int numSent = 0;
    while (size) {
        do {
            numSent = write(request_client_to_server, buffer, size);
        } while (numSent == -1 && EINTR == errno);
        /*
        if (numSent <= 0) {
            Close();
            break;
        }
        */
        size -= numSent;
        buffer += numSent;
    }
    return numSent;
}

bool FIFOClient::send(void *inData, size_t size) {
	if (!(sendData(inData, size) > 0)) {
		LOGE("Error writing to the request FIFO pipe");
        return false;
	}
	return true;
}

int FIFOClient::recvData(void *outData, size_t size) {
    char *buffer = (char *) outData;
    int numReceived = read(response_server_to_client, buffer, size);
    return numReceived;
}

bool FIFOClient::receive(void *outData, size_t size) {
	if (recvData(outData, size) == -1) {
		LOGE("Error reading from the response FIFO pipe");
   		return false;
	}
	return true;
}

void FIFOClient::Close() {
    created = false;
    connected = false;
    if (request_client_to_server > 0) {
        close(request_client_to_server);
	}
    if (response_server_to_client > 0) {
        close(response_server_to_client);
	}
}

