#ifndef FIFOSERVER_H
#define FIFOSERVER_H

#include "Const.h"

class FIFOServer {
public:
    int fifoServerFd;
    bool isCreated;
    
    FIFOServer();

    bool Create(const char* s1, const char* s2);
    bool Accept();
    bool Bind(const char* s1, const char* s2);
    bool Listen();
    void Close();

    int sendData(void *inData, size_t size);
	bool send(void* inData, size_t size);

    int recvData(void *outData, size_t size);
	bool receive(void* outData, size_t size);
};


#endif
