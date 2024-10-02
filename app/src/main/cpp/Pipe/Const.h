#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <errno.h>


static auto FIFO_REQUEST = "/data/user/0/com.learn.gg.fifo/files/fifo_client";
static auto FIFO_RESPONSE = "/data/user/0/com.learn.gg.fifo/files/fifo_server";


static int request_client_to_server;
static int response_server_to_client;
