#include "client.hpp"

extern "C" Client *createClientInstance(int port)
{
    return new Client(port);
}

extern "C" void startConnection(Client *client)
{
    client->startConnection();
}

extern "C" void sendDirection(Client *client, float x, float y)
{
    client->sendDirection(x, y);
}

extern "C" void sendShoot(Client *client)
{
    client->sendShoot();
}

extern "C" int receiveData(Client *client, uint8_t *buffer, int size)
{
    return client->receiveData(buffer, size);
}