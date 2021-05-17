#pragma once

#include "../../game_logic/src/gameState.hpp"
#include "../../protobuf/src/game.pb.h"
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

class Client
{
private:
    int socket;
    /* data */
public:
    Client(int connectedSocket);
    ~Client();
    void sendGameStateToClient(const GameState &gs, int client_id);
    void sendStatusToClient(Status gs);
    int receiveFromClient(uint8_t *buffer, size_t size);
};
