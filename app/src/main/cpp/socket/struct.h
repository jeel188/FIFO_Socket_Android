//
// Created by Patel on 01-10-2024.
//

#ifndef SSL_BYPASSER_REQUEST_H
#define SSL_BYPASSER_REQUEST_H


struct Request {
    char pkg_name[100];
    char app_name[100];
    int code;
};
struct Response {
    char developedBy[100];
    char Message[100];
    bool isSuccess;
};


#endif
