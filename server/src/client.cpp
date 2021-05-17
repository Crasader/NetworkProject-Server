#include "client.hpp"

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
        uint8_t buffer1[4096 + 1];
        GameStateMessage gsm;
        buffer1[0] = 0; // set flag 0 to indicate client that this message contain gamestate
        PlayerStat *ps = gsm.mutable_playerstat();
        ps->set_x(gs.player1->getPosition().x);
        ps->set_y(gs.player1->getPosition().y);
        ps->set_health(gs.player1->getHealth());

        for (int i = 0; i < gs.bulletList1.size(); i++)
        {
            Bullet *b = gsm.add_bullets();
            b->set_x(gs.bulletList1[i]->getPosition().x);
            b->set_y(gs.bulletList1[i]->getPosition().y);
            b->set_vx(gs.bulletList1[i]->getDirection().x);
            b->set_vy(gs.bulletList1[i]->getDirection().y);
        }

        int size = gsm.ByteSizeLong();
        gsm.SerializeToArray(&buffer1[1], size);
        int byte_sent = send(socket, buffer1, size, 0);
        if (byte_sent < 0)
            printf("send status fail!\n");
    }
    else
    {
        //send client
        uint8_t buffer2[4096 + 1];

        GameStateMessage gsm2;
        buffer2[0] = 0;
        PlayerStat *ps = gsm2.mutable_playerstat();

        ps->set_x(gs.player2->getPosition().x);
        ps->set_y(gs.player2->getPosition().y);
        ps->set_health(gs.player2->getHealth());

        for (int i = 0; i < gs.bulletList2.size(); i++)
        {
            Bullet *b = gsm2.add_bullets();
            b->set_y(gs.bulletList2[i]->getPosition().y);
            b->set_x(gs.bulletList2[i]->getPosition().x);
            b->set_vx(gs.bulletList2[i]->getDirection().x);
            b->set_vy(gs.bulletList2[i]->getDirection().y);
        }

        int size = gsm2.ByteSizeLong();
        gsm2.SerializeToArray(&buffer2[1], size);
        int byte_sent = send(socket, buffer2, size, 0);
        if (byte_sent < 0)
            printf("send status fail!\n");
    }
}

void Client::sendStatusToClient(Status gs)
{
    uint8_t buffer[4096 + 1];
    buffer[0] = 1; // flag 1 to indicate Status is sent
    int sz = gs.ByteSizeLong();
    gs.SerializeToArray(&buffer[1], sz);

    int byte_sent = send(socket, buffer, 0, sz);
    if (byte_sent < 0)
        printf("Sending from server error!");
}

int Client::receiveFromClient(uint8_t *buffer, size_t size)
{
    size_t received_size = recv(socket, buffer, size, MSG_DONTWAIT);

    return received_size;
}