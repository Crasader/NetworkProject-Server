#pragma once

#include <arpa/inet.h>
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

#include "../../protobuf/src/game.pb.h"

class Client
{
private:
    static constexpr int buffer_length = 4096 + 2; // 2 additional byte for storing flags

    int client_socket;
    struct sockaddr_in server_address;
    char sendline[buffer_length];
    int bytes_sent, bytes_received;
    /* data */
public:
    Client(int port_number);
    ~Client();
    void startConnection();
    void sendDirection(float x, float y);
    void sendShoot();
    int receiveData(uint8_t *buffer, int size);
    void receivePlayerState(uint8_t *buffer, int size);
    void receiveStatus(uint8_t *buffer, int size);
};
