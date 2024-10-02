#ifndef FIFOCLIENT_H
#define FIFOCLIENT_H

#include "Const.h"

class FIFOClient {
public:
    int fifoClientFd;
    bool created,connected;
    
    FIFOClient();

    bool Create();
    bool Connect(const char* s1, const char* s2);
    void Close();

    int sendData(void *inData, size_t size);
    bool send(void* inData, size_t size);

    int recvData(void *outData, size_t size);
    bool receive(void* outData, size_t size);
};

#endif
