#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

class SocketRW {
  public:
    SocketRW();
    char* sread();
    void supdate();
  private:
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[1024] = {0};
};