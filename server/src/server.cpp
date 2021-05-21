#include "server.hpp"

Server::Server(int port_number)
{
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port_number);

    //bind to the port
    if (bind(listen_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != -1)
    {
        printf("Bind successfully...\n");
    }
    else
    {
        printf("Error binding to socket.\n");
        perror("Error: ");
        exit(-1);
    }
    current_connection = 0;
    current_room = 0;
    is_waiting = 0;
}

Server::~Server()
{
}

void Server::mainLogic()
{
    listen(listen_socket, max_rooms * num_per_room);
    printf("Start listening...");
    //start logic
    while (current_connection < max_rooms * num_per_room)
    {
        client_length[current_connection] = sizeof(client_address[current_connection]);

        if ((client_sockets[current_connection] = accept(listen_socket, (struct sockaddr *)&client_address[current_connection], &client_length[current_connection])) == -1)
            perror("Error accepting connection!\n");
        else
        {
            int client_id = is_waiting;
            printf("Connection established for client %d!\n", current_connection);

            if (is_waiting % 2)
            {
                printf("Entering the room!\n");
                rooms.emplace_back(client_sockets[current_connection - 1], client_sockets[current_connection]);
                rooms[current_room].startRoom();
                current_room++;
            }
            else
            {
                printf("Send wait signal for player 1!\n");
                Status c1_status;
                c1_status.set_status(0); // waiting
                // uint8_t buffer[4096];
                // int sz = c1_status.ByteSizeLong();
                // c1_status.SerializeToArray(buffer, sz);

                // int byte_sent = send(client_sockets[current_connection], buffer, 0, sz);
                // if (byte_sent < 0)
                //     printf("Sending from server error!");

                uint8_t buffer[4096 + 1];
                buffer[0] = 1; // flag 1 to indicate Status is sent
                int sz = c1_status.ByteSizeLong();
                printf("Size of message: %d\n", sz);
                c1_status.SerializeToArray(&buffer[1], sz);

                printf("Start sending\n");
                int byte_sent = send(client_sockets[current_connection], buffer, sz + 1, 0);

                if (byte_sent < 0)
                    printf("Sending from server error!\n");
                else
                    printf("Sending from server success: %d bytes sent!\n", byte_sent);
            }

            printf("Current Connection: %d\n", current_connection);

            current_connection++;
            is_waiting = ++is_waiting % 2;
        }
    }
}
