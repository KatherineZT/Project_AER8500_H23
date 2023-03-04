class SocketRW {
  public:
    SocketRW();
    sread();
    supdate();
  private:
    int server_fd, new_socket;
    struct sockaddr_in address;
};