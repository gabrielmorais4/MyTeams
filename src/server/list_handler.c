/*
** EPITECH PROJECT, 2023
** B-NWP-400-MAR-4-1-myteams-niels.ouvrard
** File description:
** list_handler
*/

#include "my_server.h"

int list_replies(server **serv, client *cli, int sd)
{
    char request[1024], to_send[1024];
    strcpy(request, "SELECT * FROM replies WHERE thread = ?;");
    sqlite3_prepare_v2((*serv)->db, request, -1, &(*serv)->stmt, NULL);
    sqlite3_bind_text((*serv)->stmt, 1, cli->thread, -1, SQLITE_STATIC);
    strcpy(to_send, CODE_225);
    while (sqlite3_step((*serv)->stmt) == SQLITE_ROW) {
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 1));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 2));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 4));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 3));
        strcat(to_send, "\n");
    }
    sqlite3_finalize((*serv)->stmt);
    send(sd, to_send, strlen(to_send) + 1, MSG_NOSIGNAL);
    return 0;
}

int list_threads(server **serv, client *cli, int sd)
{
    char request[1024], to_send[1024];
    strcpy(request, "SELECT * FROM threads WHERE channel = ?;");
    sqlite3_prepare_v2((*serv)->db, request, -1, &(*serv)->stmt, NULL);
    sqlite3_bind_text((*serv)->stmt, 1, cli->channel, -1, SQLITE_STATIC);
    strcpy(to_send, CODE_224);
    while (sqlite3_step((*serv)->stmt) == SQLITE_ROW) {
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 1));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 3));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 6));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 4));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 5));
        strcat(to_send, "\n");
    }
    sqlite3_finalize((*serv)->stmt);
    send(sd, to_send, strlen(to_send) + 1, MSG_NOSIGNAL);
    return 0;
}

int list_channel(server **serv, client *cli, int sd)
{
    char request[1024], to_send[1024];
    strcpy(request, "SELECT * FROM channels WHERE team = ?;");
    sqlite3_prepare_v2((*serv)->db, request, -1, &(*serv)->stmt, NULL);
    sqlite3_bind_text((*serv)->stmt, 1, cli->team, -1, SQLITE_STATIC);
    strcpy(to_send, CODE_223);
    while (sqlite3_step((*serv)->stmt) == SQLITE_ROW) {
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 1));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 3));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 4));
        strcat(to_send, "\n");
    }
    sqlite3_finalize((*serv)->stmt);
    send(sd, to_send, strlen(to_send) + 1, MSG_NOSIGNAL);
    return 0;
}

int list_team(server **serv, client *cli, int sd)
{
    char request[1024], to_send[4096];
    strcpy(request, "SELECT * FROM teams;");
    sqlite3_prepare_v2((*serv)->db, request, -1, &(*serv)->stmt, NULL);
    strcpy(to_send, CODE_222);
    while (sqlite3_step((*serv)->stmt) == SQLITE_ROW) {
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 1));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 2));
        strcat(to_send, "\n");
        strcat(to_send, sqlite3_column_text((*serv)->stmt, 3));
        strcat(to_send, "\n");
    }
    sqlite3_finalize((*serv)->stmt);
    send(sd, to_send, strlen(to_send) + 1, MSG_NOSIGNAL);
    return 0;
}
