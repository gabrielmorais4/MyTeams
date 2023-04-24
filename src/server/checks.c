/*
** EPITECH PROJECT, 2023
** B-NWP-400-MAR-4-1-myteams-niels.ouvrard
** File description:
** user_check
*/

#include "my_server.h"

bool args_check(char **command, int nb_args, int sd)
{
    int i = 0;

    while (command[i] != NULL)
        i++;
    if (i != nb_args) {
        send(sd, CODE_590, strlen(CODE_590) + 1, MSG_NOSIGNAL);
        return false;
    }
    return true;
}

// an error exist for that ?
bool user_connected(client *current_client)
{
    if (current_client->is_logged == true) {
        send(current_client->socket, CODE_331,
        strlen(CODE_331) + 1, MSG_NOSIGNAL);
        return true;
    }
    return false;
}

bool user_not_connected(client *current_client)
{
    if (current_client->is_logged == false) {
        send(current_client->socket, CODE_504,
        strlen(CODE_504) + 1, MSG_NOSIGNAL);
        return true;
    }
    return false;
}

bool check_file_exist(char *file_path)
{
    FILE *fd = fopen(file_path, "r");
    if (fd == NULL) {
        return false;
    } else {
        fclose(fd);
        return true;
    }
}
