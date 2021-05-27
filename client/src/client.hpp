#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class Client
{
private:
    static constexpr int buffer_length = 4096 + 5;

    int client_socket;
    struct sockaddr_in server_address;
    int bytes_sent, bytes_received;
    /* data */
public:
    Client(int port_number);
    ~Client();
    void startConnection();
    void sendDirection(uint8_t *buffer, int size);
    void sendShoot(uint8_t *buffer, int size);
    int receiveData(uint8_t *buffer, int size);
    int closeConnection();
};
