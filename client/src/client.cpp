#include "client.hpp"

Client::Client(int port_number)
{
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); //temporary hardcode for localhost
    server_address.sin_port = htons(port_number);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // startConnection();
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

void Client::sendDirection(uint8_t *buffer, int size)
{
    // memset(sendline, 0, buffer_length);
    // MovingDirection md;
    // md.set_vx(x);
    // md.set_vy(y);

    // int sz = md.ByteSizeLong();
    // sendline[0] = 0;
    // md.SerializeToArray(&sendline[1], sz);
    uint8_t full_buff[size + 1 + sizeof(int)];
    full_buff[0] = 0; //set flag 0 to indicate the server that it is updateDirection message
    uint8_t *buf_2 = full_buff + 1;
    int *length = (int *)buf_2;
    *length = size;
    //copy the proto-buffer to full buffer
    memcpy(full_buff + 1 + sizeof(int), buffer, size);
    bytes_sent = send(client_socket, full_buff, size + 1 + sizeof(int), MSG_DONTWAIT);
    if (bytes_sent < 0)
    {
        if (errno == EWOULDBLOCK)
            printf("send direction fail!\n");
        else
        {
            perror("Error: ");
            exit(0);
        }
    }
}

void Client::sendShoot(uint8_t *buffer, int size)
{
    // memset(sendline, 0, buffer_length);
    // IsShot shoot;

    // int sz = shoot.ByteSizeLong();
    // sendline[0] = 1; //set flag 0 to indicate the server that it is isShot message
    // shoot.SerializeToArray(&sendline[1], sz);

    uint8_t full_buff[size + 1 + sizeof(int)];
    full_buff[0] = 1; //set flag 1 to indicate the server that it is sendShoot message
    uint8_t *buf_2 = full_buff + 1;
    int *length = (int *)buf_2;
    *length = size;
    //copy the proto-buffer to full buffer
    memcpy(full_buff + 1 + sizeof(int), buffer, size);
    bytes_sent = send(client_socket, full_buff, size + 1 + sizeof(int), MSG_DONTWAIT);
    if (bytes_sent < 0)
    {
        if (errno == EWOULDBLOCK)
            printf("send shoot fail!\n");
        else
        {
            perror("Error: ");
            exit(0);
        }
    }
}

int Client::receiveData(uint8_t *buffer, int size)
{
    bytes_received = recv(client_socket, buffer, size, MSG_DONTWAIT);
    if (bytes_received > 0)
    {
        printf("Receive %d bytes\n", bytes_received);
        // switch (buffer[0])
        // {
        // case 0:
        //     receivePlayerState(buffer + 1, bytes_received - 1);
        //     return 0; //the gamestate message
        //     break;
        // case 1:
        //     receiveStatus(buffer + 1, bytes_received - 1);
        //     return 1; // the status message
        //     break;
        // default:
        //     break;
        // }
    }
    else
    {
        if (errno == EWOULDBLOCK)
            printf("receive null!\n");
        else
        {
            perror("Error: ");
            exit(0);
        }
    }
    return bytes_received;
}

int Client::closeConnection()
{
    //close connection
    return close(client_socket);
}

// void Client::receivePlayerState(uint8_t *buffer, int size)
// {
//     GameStateMessage gs;
//     gs.ParseFromArray(buffer, size);

//     printf("The info of game state:\n heath: %d, x: %f, y: %f\n",
//            gs.playerstat().health(),
//            gs.playerstat().x(),
//            gs.playerstat().y());
// }

// void Client::receiveStatus(uint8_t *buffer, int size)
// {
//     Status status;
//     status.ParseFromArray(buffer, size);

//     printf("The info of status: %d\n", status.status());
// }