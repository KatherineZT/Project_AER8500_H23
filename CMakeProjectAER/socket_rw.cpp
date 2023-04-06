// Server side C/C++ program to demonstrate Socket
// programming
#include "socket_rw.h"

#define PORT 8080

SocketRW::SocketRW() {
    int opt = 1;
    int addrlen = sizeof(address);
    translator = new A429();
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    ioctl(new_socket, FIONBIO, &opt);
}

a429_request* SocketRW::sread() {
    memset(buffer, 0, sizeof(buffer));
    read(new_socket, buffer, 1024); //we expect to receive in hex format: "0FA600FF 4E9F020A"
    return translator->translateReceive(buffer);
}

void SocketRW::supdate(int altitude, float climbingSpeed, int state) {
    a429_request state_altitude;
    a429_request climbing;

    char* packet;

    state_altitude.label = 1;
    state_altitude.data1 = state;
    state_altitude.data2 = altitude;
    state_altitude.ssm = 3;

    climbing.label = 2;
    if (climbingSpeed < 0) {
        climbing.data1 = climbingSpeed;
        climbing.ssm = 0;
    }
    else {
        climbing.data1 = climbingSpeed;
        climbing.ssm = 3;
    }

    packet = translator->translateSend(state_altitude);
    send(new_socket, packet, strlen(packet), 0);

    packet = translator->translateSend(climbing);
    send(new_socket, packet, strlen(packet), 0);
}

void SocketRW::Close() {
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
}