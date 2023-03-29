/*
** EPITECH PROJECT, 2023
** B-NWP-400-MAR-4-1-myftp-gabriel.de-souza-morais
** File description:
** client_handler
*/

#include "my.h"

void initialize_client(client **clients)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        (*clients)[i].socket = -1;
        (*clients)[i].uuid_text = malloc(sizeof(uuid_t) * 2 + 5);
        (*clients)[i].username = NULL;
    }
}

void add_client(client **clients, int socket_fd, struct sockaddr_in address)
{
    for (int i = 0; i != MAX_CLIENTS; i++) {
        if ((*clients)[i].socket == -1) {
            (*clients)[i].socket = socket_fd;
            (*clients)[i].address = address;
            (*clients)[i].username = NULL;
            break;
        }
    }
}

void remove_client(client *clients, int client_fd)
{
    for (int i = 0; i != MAX_CLIENTS; i++) {
        if (clients->socket == client_fd) {
            clients->socket = -1;
            uuid_clear(clients->uuid);
            clients->uuid_text = malloc(sizeof(uuid_t) * 2 + 5);
            clients->username = NULL;
            clients->is_logged = false;
            break;
        }
    }
}

void handle_new_connection(server **serv, client **clients, fd_set copy_fds)
{
    int new_socket_fd;
    if (FD_ISSET((*serv)->socket_fd, &copy_fds)) {
        (*serv)->addlen = sizeof((*serv)->address);
        new_socket_fd = accept((*serv)->socket_fd,
        (struct sockaddr *)&(*serv)->address,
        (socklen_t *)&(*serv)->addlen);
        if (new_socket_fd < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        FD_SET(new_socket_fd, &(*serv)->readfds);
        if (new_socket_fd > (*serv)->max_fds) {
            (*serv)->max_fds = new_socket_fd;
        }
        add_client(clients, new_socket_fd, (*serv)->address);
        send(new_socket_fd, "220 Service ready for new user.\n", 32, 0);
    }
}

int client_communication(server **serv, client **clients, fd_set copy_fds)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        int sd = (*clients)[i].socket;
        if (FD_ISSET(sd, &copy_fds)) {
            (*serv)->command = get_command(sd);
            command_handler(serv, clients, &(*clients)[i], sd);
        }
    }
}