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
        for (int i = 0; i < 2; i++)
        {
            while (1)
            {
                uint8_t *buff = buffer.data();
                int flag = clients[i].receiveFromClient(buff, 1); //read the flag first
                int status;
                if (flag > 0)
                {
                    flag = clients[i].receiveFromClient(buff + 1, sizeof(int));
                    printf("Receive message!!! Flag %d\n", buff[0]);
                    status = buff[0];
                }
                else
                    break;
                // uint8_t message[4096];
                // memcpy(message, buff + 1, sizeof(buff) - 1); //extract the needed infos
                if (flag > 0)
                {
                    uint8_t *l = buff + 1;
                    int *length = (int *)l;
                    flag = clients[i].receiveFromClient(buff + 1 + sizeof(int), *length);

                    if (flag >= 0)
                        switch (status)
                        {
                        case 0: //the direction of client
                            gm.updatePlayerDir(buff + 1 + sizeof(int), i, *length);
                            break;
                        case 1: //the client is shooting
                            printf("recv shoot...");
                            gm.addNewBullet(buff + 1 + sizeof(int), i);
                            break;
                        }
                }
            }
        }
        int status = gm.update(deltaTime.count());
        switch (status)
        {
        case 0: // player 1 win
        {
            isRunning = false;
            Status s1;
            s1.set_status(2); //send win signal to p1
            clients[0].sendStatusToClient(s1);
            Status s2;
            s2.set_status(3); //send lose signal to p2
            clients[1].sendStatusToClient(s2);
            // sleep 2 frame to ensure all maessage recv by client
            usleep(20 * 1000);

            break;
        }
        case 1: // player 2 win
        {
            isRunning = false;
            Status s1;
            s1.set_status(3); //send lose signal to p1
            clients[0].sendStatusToClient(s1);
            Status s2;
            s2.set_status(2); //send win signal to p2
            clients[1].sendStatusToClient(s2);

            // sleep 2 frame to ensure all maessage recv by client
            usleep(20 * 1000);

            break;
        }
        default:
            break;
        }

        //send data to client to render
        // now the data contained in gameState inside gm, access it and send that data to client
        if (isRunning)
            for (size_t i = 0; i < 2; i++)
            {
                clients[i].sendGameStateToClient(gm.getState(), i);
            }

        //sleep a short time to slow down the update process
        usleep(20 * 1000);
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