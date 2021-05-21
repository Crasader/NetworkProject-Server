#include "room.hpp"

Room::Room(int socket_c1, int socket_c2) : lastUpdate{}, gm{}, clients{socket_c1, socket_c2}, pid{}, buffer(4096 + 2), isRunning{false}
{
    printf("Socket for client 1: %d\n", socket_c1);
    printf("Socket for client 2: %d\n", socket_c2);
}

void *Room::threadLoop(void *param)
{
    Room *room = (Room *)param;
    room->run();

    return NULL;
}

void Room::run()
{
    printf("Thread run...\n");
    while (isRunning)
    {
        using clk = std::chrono::high_resolution_clock;
        using namespace std::chrono;
        clk::time_point now = clk::now();
        duration<float> deltaTime = duration_cast<duration<float>>(now - lastUpdate);
        lastUpdate = now;
        uint8_t *buff = buffer.data();
        for (int i = 0; i < 2; i++)
        {
            int flag = clients[i].receiveFromClient(buff, buffer.size());
            if (flag > 0)
            {
                // uint8_t message[4096];
                // memcpy(message, buff + 1, sizeof(buff) - 1); //extract the needed infos
                switch (buffer[0])
                {
                case 0: //the direction of client
                    gm.updatePlayerDir(buff + 1, i, flag - 1);
                    break;
                case 1: //the client is shooting
                    gm.addNewBullet(buff + 1, i);
                    break;
                }
            }
        }
        int status = gm.update(deltaTime.count());
        switch (status)
        {
        case 0: // player 1 win
        {
            isRunning = false;
            Status s;
            s.set_status(2); //send win signal to p1
            clients[0].sendStatusToClient(s);
            s.set_status(3); //send lose signal to p2
            clients[1].sendStatusToClient(s);
            break;
        }
        case 1: // player 2 win
        {
            isRunning = false;
            Status s;
            s.set_status(3); //send lose signal to p1
            clients[0].sendStatusToClient(s);
            s.set_status(2); //send win signal to p2
            clients[1].sendStatusToClient(s);
            break;
        }
        default:
            break;
        }

        //send data to client to render
        // now the data contained in gameState inside gm, access it and send that data to client
        for (size_t i = 0; i < 2; i++)
        {
            clients[i].sendGameStateToClient(gm.getState(), i);
        }
    }
}

void Room::startRoom()
{
    //send start signal to 2 clients
    Status s_status;
    s_status.set_status(1); // starting
    for (size_t i = 0; i < 2; i++)
    {
        clients[i].sendStatusToClient(s_status);
    }

    isRunning = true;
    lastUpdate = std::chrono::high_resolution_clock::now();

    int res = pthread_create(&pid, NULL, threadLoop, this);
    if (res < 0)
    {
        perror("Error creating thread\n");
        exit(-1);
    }
}