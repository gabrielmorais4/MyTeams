/*
** EPITECH PROJECT, 2023
** B-NWP-400-MAR-4-1-myteams-niels.ouvrard
** File description:
** main
*/

#include "my_client.h"
#include <signal.h>

static int keepRunning = 1;

void int_handler(int dummy)
{
    keepRunning = 0;
}

int loop_client (client *cli, fd_set read_fds, int *server_reply,
struct timeval tv)
{
    char *message = NULL;
    fd_set tmp_fds = read_fds;
    if (select(cli->sock + 1, &tmp_fds, NULL, NULL, &tv) < 0)
        return 0;
    if (FD_ISSET(cli->sock, &tmp_fds)) {
        *server_reply = handle_server_response(cli);
    }
    if (*server_reply == -1 || *server_reply == 84) {
        return -1;
    }
    if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
        message = loop_get_message(NULL);
        send(cli->sock, message, strlen(message), 0);
        free(message);
    }
    return 0;
}

int loop(client *cli)
{
    struct sigaction sa;
    sa.sa_handler = int_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(cli->sock, &read_fds); FD_SET(STDIN_FILENO, &read_fds);
    struct timeval tv;
    tv.tv_sec = 0; tv.tv_usec = 10000;
    int server_reply = 0;
    while (keepRunning)
        if (loop_client(cli, read_fds, &server_reply, tv) == -1)
            return server_reply;
    return EXIT_CTRL_C;
}

int free_and_return (client *cli, int server_reply)
{
    if (server_reply == EXIT_CTRL_C) {
        if (cli->uuid)
            free(cli->uuid);
        if (cli->name) {
            free(cli->name);
            send(cli->sock, "/logout", strlen("/logout\n"), 0);
        }
    }
    free(cli->funct_client);
    close(cli->sock);
    if (server_reply == 84)
        return 84;
    return 0;
}

int client_funct (int ac, char **av)
{
    client cli;
    cli.name = NULL;
    cli.uuid = NULL;
    cli.is_connected = false;
    struct sockaddr_in server;
    cli.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cli.sock == -1)
        printf("Could not create socket");
    server.sin_addr.s_addr = inet_addr(av[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(av[2]));
    if (connect(cli.sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }
    cli.funct_client = array_struct();
    int server_reply = loop(&cli);
    return free_and_return(&cli, server_reply);
}
