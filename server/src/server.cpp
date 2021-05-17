#include "server.hpp"

Server::Server(int port_number)
{
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port_number);

    //bind to the port
    if (bind(listen_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        printf("Bind successfully...\n");
        exit(-1);
    }
    else
    {
        printf("Error binding to socket.\n");
        perror("Error: ");
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
            printf("Connecttion established for client %d!\n", current_connection);

            if (is_waiting % 2)
            {
                rooms.push_back(Room(client_sockets[current_connection - 1], client_sockets[current_connection]));
                rooms[current_room].startRoom();
                current_room++;
            }
            else
            {
                Status c1_status;
                c1_status.set_status(0); // waiting
                uint8_t buffer[4096];
                int sz = c1_status.ByteSizeLong();
                c1_status.SerializeToArray(buffer, sz);

                int byte_sent = send(client_sockets[current_connection], buffer, 0, sz);
                if (byte_sent < 0)
                    printf("Sending from server error!");
            }
            current_connection++;
            is_waiting = ++is_waiting % 2;
        }
    }
}
