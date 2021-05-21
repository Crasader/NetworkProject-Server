#pragma once

#include <chrono>
#include <pthread.h>

#include "../../game_logic/src/gameManager.hpp"
#include "client.hpp"

class Room
{
private:
    std::chrono::high_resolution_clock::time_point lastUpdate;
    GameManager gm;
    Client clients[2];
    pthread_t pid;
    std::vector<uint8_t> buffer;
    bool isRunning;
    static void *threadLoop(void *param);
    void run();

public:
    Room(const Room &) = delete; // copy to constructor -> delete
    Room &operator=(const Room &) = delete;
    Room(Room &&) = default;
    Room &operator=(Room &&) = default;
    Room(int socket_c1, int socket_c2);
    void startRoom();
};
