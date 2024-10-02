#include "FIFOServer.h"
#include "Log.h"

FIFOServer::FIFOServer(){
    isCreated = false;
}

bool FIFOServer::Create(const char* s1, const char* s2) {
    int op,os;
    unlink(s1);
    unlink(s2);
    /* create the FIFO (named pipe) */
    op = mkfifo(/*FIFO_REQUEST*/ s1, 0666);
    os = mkfifo(/*FIFO_RESPONSE*/ s2, 0666);
    if(os == -1 && op == -1){
       isCreated = false;
       return isCreated;
    }
    isCreated = true;
    return isCreated;
}

bool FIFOServer::Bind(const char* s1, const char* s2) {
    request_client_to_server = open(/*FIFO_REQUEST*/s1, O_RDONLY);
	if (request_client_to_server == -1) {
        LOGE("SERVER: Error opening the request FIFO pipe for reading");
        return false;
    }
    response_server_to_client = open(/*FIFO_RESPONSE*/s2, O_WRONLY);
	if (response_server_to_client == -1) {
        LOGE("SERVER: Error opening the response FIFO pipe for writing");
        return false;
    }
    return true;
}

bool FIFOServer::Listen() {
    return true;
}

bool FIFOServer::Accept() {
    return true;
}

int FIFOServer::sendData(void *inData, size_t size) {
    char *buffer = (char *) inData;
    int numSent = 0;
    while (size) {
        do {
            numSent = write(response_server_to_client, buffer, size);
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

bool FIFOServer::send(void* inData, size_t size) {
	if (!(sendData(inData, size) > 0)) {
		LOGE("SERVER : Error writing to the response FIFO pipe");
        return false;
	}
	return true;
}

int FIFOServer::recvData(void *outData, size_t size) {
    char *buffer = (char *) outData;
    int numReceived = read(request_client_to_server, buffer, size);
    return numReceived;
}

bool FIFOServer::receive(void* outData, size_t size) {
	if (recvData(outData, size) == -1) {
		LOGE("SERVER: Error reading from the request FIFO pipe");
   		return false;
	}
	return true;
}

void FIFOServer::Close() {
    if (response_server_to_client > 0) {
        close(response_server_to_client);
	}
    if (request_client_to_server > 0) {
        close(request_client_to_server);
	}
    unlink(FIFO_REQUEST);
    unlink(FIFO_RESPONSE);
}

/* --------EXAMPLE-------- */
/*
int main()
{
    char buf[BUFSIZ];
    FIFOServer server;
    
    if(!server.Create()){
        printf("Server Not Created.\n");
        return 0;
    }
    
    if(!server.Bind()){
        printf("Server Not Bind.\n");
        return 0;
    }
    
    printf("Server ON.\n\n");

    while (true) {
        server.receive((void*)&buf, sizeof(buf));

        if (strcmp("exit", buf) == 0) {
            printf("Server OFF.\n");
            server.Close();
            break;
        }
        else if (strcmp("", buf) != 0) {
            printf("Received message: %s\n", buf);
			
			char reply[BUFSIZ];
        	sprintf(reply, "Returning form server : %s", buf);
            server.send(reply, sizeof(reply));
			printf("Sent reply: %s\n", reply);
        }

        /* clean buf from any data *
        memset(buf, 0, sizeof(buf));
        sleep(3);
    }
    return 0;
}
*/
