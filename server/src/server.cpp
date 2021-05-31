#include "server.hpp"

Server::Server(int port_number)
{
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    int tr = 1;

    //make the sock address reusable after destroyed
    int flag = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int));

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
    listen(listen_socket, INT_MAX);
    printf("Start listening...");
    //start logic
    while (1)
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

                uint8_t buffer[4096 + 5];
                buffer[0] = 1; // flag 1 to indicate Status is sent
                int sz = c1_status.ByteSizeLong();
                uint8_t *buf_2 = buffer + 1;
                int *length = (int *)buf_2;
                *length = sz;
                printf("The size of status: %d\n", sz);
                c1_status.SerializeToArray(&buffer[1 + sizeof(int)], sz);

                printf("Start sending\n");
                int byte_sent = send(client_sockets[current_connection], buffer, sz + 1 + sizeof(int), 0);

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
