#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "a429_lib.h"

class SocketRW {
  public:
    SocketRW();
    a429_request* sread();
    void supdate(int altitude, float climbingSpeed, int state);
  private:
    int server_fd, new_socket;
    A429* translator;
    struct sockaddr_in address;
    char buffer[1024] = {0};
};