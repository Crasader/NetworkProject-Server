#include "client.hpp"
#include "../../game_logic/src/bullet.hpp"
#include "../../game_logic/src/playerObject.hpp"

Client::Client(int connectedSocket)
{
    socket = connectedSocket;
}

Client::~Client()
{
}

void Client::sendGameStateToClient(const GameState &gs, int client_id)
{
    if (client_id == 0)
    {
        uint8_t buffer1[4096 + 5];
        GameStateMessage gsm;
        buffer1[0] = 0; // set flag 0 to indicate client that this message contain gamestate
        PlayerStat *ps = gsm.mutable_playerstat();
        ps->set_x(((PlayerObject *)gs.player1)->getPosition().x);
        ps->set_y(((PlayerObject *)gs.player1)->getPosition().y);
        ps->set_health(((PlayerObject *)gs.player1)->getHealth());

        for (int i = 0; i < gs.bulletList1.size(); i++)
        {
            Bullet *b = gsm.add_bullets();
            b->set_x(((BulletObject *)gs.bulletList1[i])->getPosition().x);
            b->set_y(((BulletObject *)gs.bulletList1[i])->getPosition().y);
            b->set_vx(((BulletObject *)gs.bulletList1[i])->getDirection().x);
            b->set_vy(((BulletObject *)gs.bulletList1[i])->getDirection().y);
        }

        int size = gsm.ByteSizeLong();

        uint8_t *buf_2 = buffer1 + 1;
        int *length = (int *)buf_2;
        *length = size;
        printf("The size of gameState: %d\n", size);

        gsm.SerializeToArray(&buffer1[1 + sizeof(int)], size);
        int byte_sent = send(socket, buffer1, size + 1 + sizeof(int), MSG_DONTWAIT);
        if (byte_sent < 0)
        {
            if (errno == EWOULDBLOCK)
                printf("send GameState fail!\n");
            else
            {
                perror("Error: ");
                exit(0);
            }
        }
    }
    else
    {
        //send client
        uint8_t buffer2[4096 + 5];

        GameStateMessage gsm2;
        buffer2[0] = 0;
        PlayerStat *ps = gsm2.mutable_playerstat();

        ps->set_x(((PlayerObject *)gs.player2)->getPosition().x);
        ps->set_y(((PlayerObject *)gs.player2)->getPosition().y);
        ps->set_health(((PlayerObject *)gs.player2)->getHealth());

        for (int i = 0; i < gs.bulletList2.size(); i++)
        {
            Bullet *b = gsm2.add_bullets();
            b->set_y(((BulletObject *)gs.bulletList2[i])->getPosition().y);
            b->set_x(((BulletObject *)gs.bulletList2[i])->getPosition().x);
            b->set_vx(((BulletObject *)gs.bulletList2[i])->getDirection().x);
            b->set_vy(((BulletObject *)gs.bulletList2[i])->getDirection().y);
        }

        int size = gsm2.ByteSizeLong();
        uint8_t *buf_2 = buffer2 + 1;
        int *length = (int *)buf_2;
        *length = size;
        printf("The size of game State: %d\n", size);

        gsm2.SerializeToArray(&buffer2[1 + sizeof(int)], size);
        int byte_sent = send(socket, buffer2, size + 1 + sizeof(int), MSG_DONTWAIT);
        if (byte_sent < 0)
        {
            if (errno == EWOULDBLOCK)
                printf("send GameState fail!\n");
            else
            {
                perror("Error: ");
                exit(0);
            }
        }
    }
}

void Client::sendStatusToClient(Status gs)
{
    printf("The socket: %d\n", socket);
    uint8_t buffer[4096 + 5];
    buffer[0] = 1; // flag 1 to indicate Status is sent
    int sz = gs.ByteSizeLong();
    uint8_t *buf_2 = buffer + 1;
    int *length = (int *)buf_2;
    *length = sz;
    printf("The size of status: %d\n", sz);
    gs.SerializeToArray(&buffer[1 + sizeof(int)], sz);

    int byte_sent = send(socket, buffer, sz + 1 + sizeof(int), MSG_DONTWAIT);
    printf("Sent byte: %d\n", byte_sent);
    if (byte_sent < 0)
    {
        if (errno == EWOULDBLOCK)
            printf("send Status fail!\n");
        else
        {
            perror("Error: ");
            exit(0);
        }
    }
}

int Client::receiveFromClient(uint8_t *buffer, size_t size)
{
    int received_size = recv(socket, buffer, size, MSG_DONTWAIT);

    if (received_size < 0)
    {
        if (errno == EWOULDBLOCK)
        {
            printf("No message");
        }
        else
        {
            perror("Error: ");
            exit(0);
        }
    }
    else
        printf("Receive from client with socket %d: %d bytes\n", socket, received_size);
    return received_size;
}