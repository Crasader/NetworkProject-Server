#include "client.hpp"

Client::Client(int port_number)
{
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); //temporary hardcode for localhost
    server_address.sin_port = htons(port_number);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    startConnection();
}

Client::~Client()
{
}

void Client::startConnection()
{
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Error connecting to server");
        exit(-1);
    }
}

void Client::sendDirection(float x, float y)
{
    memset(sendline, 0, buffer_length);
    MovingDirection md;
    md.set_vx(x);
    md.set_vy(y);

    int sz = md.ByteSizeLong();
    sendline[0] = 0; //set flag 0 to indicate the server that it is updateDirection message
    md.SerializeToArray(&sendline[1], sz);

    bytes_sent = send(client_socket, sendline, sz + 1, 0);
    if (bytes_sent < 0)
        printf("Send direction fail!\n");
}

void Client::sendShoot()
{
    memset(sendline, 0, buffer_length);
    IsShot shoot;

    int sz = shoot.ByteSizeLong();
    sendline[0] = 1; //set flag 0 to indicate the server that it is isShot message
    shoot.SerializeToArray(&sendline[1], sz);

    bytes_sent = send(client_socket, sendline, sz + 1, 0);
    if (bytes_sent < 0)
        printf("Send isShoot fail!\n");
}

int Client::receiveData(uint8_t *buffer, int size)
{
    bytes_received = recv(client_socket, buffer, size, MSG_DONTWAIT);

    if (bytes_received > 0)
    {
        uint8_t message[4096];
        memcpy(message, buffer + 1, sizeof(buffer) - 1);
        switch (buffer[0])
        {
        case 0:
            receivePlayerState(message);
            return 0; //the gamestate message
            break;
        case 1:
            receiveStatus(message);
            return 1; // the status message
            break;
        default:
            break;
        }
    }

    return bytes_received;
}

void Client::receivePlayerState(uint8_t *buffer)
{
    GameStateMessage gs;
    gs.ParseFromArray(buffer, sizeof(buffer));

    printf("The info of game state:\n heath: %d, x: %f, y: %f\n",
           gs.playerstat().health(),
           gs.playerstat().x(),
           gs.playerstat().y());
}

void Client::receiveStatus(uint8_t *buffer)
{
    Status status;
    status.ParseFromArray(buffer, sizeof(buffer));

    printf("The info of status: %d\n", status.status());
}