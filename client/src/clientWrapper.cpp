#include "client.hpp"

extern "C" float add(float a, float b) { return a + b; }

extern "C" int sendSock() { return send(1, NULL, 1, 0); }

extern "C" Client *createClientInstance(int port)
{
    return new Client(port);
}

extern "C" void startConnection(Client *client)
{
    client->startConnection();
}

extern "C" void sendDirection(Client *client, uint8_t *buffer, int size)
{
    client->sendDirection(buffer, size);
}

extern "C" void sendShoot(Client *client, uint8_t *buffer, int size)
{
    client->sendShoot(buffer, size);
}

extern "C" int receiveData(Client *client, uint8_t *buffer, int size)
{
    return client->receiveData(buffer, size);
}

extern "C" int closeConnection(Client *client)
{
    int flag = client->closeConnection();
    free(client);
    return flag;
}
