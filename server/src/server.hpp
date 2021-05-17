#pragma once

#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../game_logic/src/gameManager.hpp"
#include "../../protobuf/src/game.pb.h"
#include "room.hpp"

class Server
{
private:
    /* data */
    static constexpr int max_rooms = 4;
    static constexpr int num_per_room = 2;
    int current_connection;
    int current_room;
    int is_waiting;
    int listen_socket, client_sockets[max_rooms * num_per_room];
    std::vector<Room> rooms;
    socklen_t client_length[max_rooms * num_per_room];
    struct sockaddr_in client_address[max_rooms * num_per_room], server_address;
    int bytes_sent, bytes_received;
    GameManager game_manager;

public:
    Server(int port_number);
    ~Server();
    void mainLogic();
    // void *handleClient(void *client_id);
};
