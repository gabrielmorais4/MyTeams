/*
** EPITECH PROJECT, 2023
** B-NWP-400-MAR-4-1-myteams-niels.ouvrard
** File description:
** command_handler
*/

#include "my_server.h"

void remove_quotes(server **serv)
{
    for (int i = 1; (*serv)->command[i] != NULL; i++) {
        char *tmp = strchr((*serv)->command[i], '\"');
        if (tmp != NULL) {
            char *new = malloc(sizeof(char) * strlen((*serv)->command[i]));
            strcpy(new, (*serv)->command[i] + 1);
            new[strlen(new) - 1] = '\0';
            free((*serv)->command[i]);
            (*serv)->command[i] = new;
        }
    }
}

void command_handler(server **serv, client **cli_list,
client *current_client, int sd)
{
    if ((*serv)->command == NULL) {
        remove_client(current_client, sd);
        close(sd);
        FD_CLR(sd, &(*serv)->readfds);
        (*serv)->command = malloc(sizeof(char *));
        (*serv)->command[0] = NULL;
        return;
    }
    if (!(*serv)->command[0])
        return;
    remove_quotes(serv);
    for (int i = 0; i < NB_COMMANDS; i++) {
        if (strcmp((*serv)->fct[i].name, (*serv)->command[0]) == 0) {
            (*serv)->fct[i].fct(serv, cli_list, current_client, sd);
            return;
        }
    }
    send(sd, CODE_591, strlen(CODE_591) + 1, MSG_NOSIGNAL);
}

char **get_command(int sd)
{
    char buffer[1024] = {0};
    recv(sd, buffer, 1024, 0);
    if (strlen(buffer) <= 0)
        return NULL;
    if (buffer[strlen(buffer) - 1] == '\n')
        buffer[strlen(buffer) - 1] = '\0';
    char **command = my_str_to_word_array(buffer);
    return command;
}

void error_sql(server *serv, char *error)
{
    fprintf(stderr, error, sqlite3_errmsg(serv->db));
    exit (84);
}

void change_status_user (server **serv, const char *uuid_text, int status)
{
    char request[1024], *err_msg;
    sprintf(request, "UPDATE users SET connected = %d WHERE uuid = '%s'",
    status, uuid_text);
    int rc = sqlite3_exec((*serv)->db, request, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}
