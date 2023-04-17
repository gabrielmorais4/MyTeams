/*
** EPITECH PROJECT, 2023
** B-NWP-400-MAR-4-1-myteams-niels.ouvrard
** File description:
** create_handler
*/

#include "my_server.h"

char *generate_uuid(void)
{
    uuid_t uuid;
    uuid_generate(uuid);
    char *uuid_text = malloc(sizeof(uuid_t) * 2 + 5);
    uuid_unparse(uuid, uuid_text);
    return uuid_text;
}

char *to_send_reply(server **serv, client *curr_cli, char *time_stamp)
{
    char *to_send = malloc(sizeof(char) * 1024);
    strcpy(to_send, CODE_214);
    strcat(to_send, curr_cli->thread);
    strcat(to_send, "\n");
    strcat(to_send, curr_cli->uuid_text);
    strcat(to_send, "\n");
    strcat(to_send, curr_cli->username);
    strcat(to_send, "\n");
    strcat(to_send, time_stamp);
    strcat(to_send, (*serv)->command[1]);
    strcat(to_send, "\n");
    return to_send;
}

char *create_reply(server **serv, client **cli_list,
                    client *curr_cli, int sd)
{
    char *body = (*serv)->command[1];
    char *user_uuid = curr_cli->uuid_text;
    char *thread_uuid = curr_cli->thread;
    char *time_stamp;
    time_t now = time(NULL);
    time_stamp = ctime(&now);
    sqlite3_prepare_v2((*serv)->db, "INSERT INTO replies (thread, user, \
    title, body, timestamp) VALUES (?, ?, ?, ?);", -1, &(*serv)->stmt, NULL);
    sqlite3_bind_text((*serv)->stmt, 1, thread_uuid, -1, SQLITE_STATIC);
    sqlite3_bind_text((*serv)->stmt, 2, user_uuid, -1, SQLITE_STATIC);
    sqlite3_bind_text((*serv)->stmt, 3, body, -1, SQLITE_STATIC);
    sqlite3_bind_text((*serv)->stmt, 4, time_stamp, -1, SQLITE_STATIC);
    sqlite3_step((*serv)->stmt);
    sqlite3_finalize((*serv)->stmt);
    server_event_reply_created(thread_uuid, user_uuid, body);
    return to_send_reply(serv, curr_cli, time_stamp);
}

char *create_team         (server **serv, client **cli_list,
                            client *cur_cli, int sd)
{
    char *uuid = generate_uuid();
    char *name = (*serv)->command[1];
    char *description = (*serv)->command[2];
    sqlite3_prepare_v2((*serv)->db,
    "INSERT INTO teams (uuid, name, description, user_uuids)\
    VALUES (?, ?, ?, ?);",
    -1, &(*serv)->stmt, NULL);
    sqlite3_bind_text((*serv)->stmt, 1, uuid, -1, SQLITE_STATIC);
    sqlite3_bind_text((*serv)->stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_text((*serv)->stmt, 3, description, -1, SQLITE_STATIC);
    sqlite3_bind_text((*serv)->stmt, 4, cur_cli->uuid_text, -1, SQLITE_STATIC);
    sqlite3_step((*serv)->stmt);
    sqlite3_finalize((*serv)->stmt);
    char *to_send = malloc(sizeof(char) * 1024);
    strcpy(to_send, CODE_211);
    strcat(to_send, uuid); strcat(to_send, "\n");
    strcat(to_send, name); strcat(to_send, "\n");
    strcat(to_send, description); strcat(to_send, "\n");
    server_event_team_created(uuid, name, cur_cli->uuid_text);
    return to_send;
}