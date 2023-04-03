/*
** EPITECH PROJECT, 2023
** B-NWP-400-MAR-4-1-myteams-niels.ouvrard
** File description:
** connection_handler
*/

#include "my_server.h"

// CODE_202 = "202 Logged out."
void logout_handler(server **serv, client *cur_cli, int sd)
{
    if (!args_check((*serv)->command, 1, sd) || user_not_connected(cur_cli))
        return;
    void *function =
    load_library_function((*serv)->lib, "server_event_user_logged_out");
    ((void (*)(char *))function)(cur_cli->uuid_text);
    char *to_send = malloc(sizeof(char) * 1024);
    sprintf(to_send, "%s\n%s\n%s", CODE_202, cur_cli->uuid_text,
    cur_cli->username);
    send(sd, to_send, strlen(to_send), 0);
    free(to_send);
    FD_CLR(sd, &(*serv)->readfds);
    close(sd);
    cur_cli->is_logged = false;
    char *file_path = malloc(strlen(cur_cli->uuid_text) + 13);
    strcpy(file_path, "data/users/");
    strcat(file_path, cur_cli->uuid_text);
    replace_line_file("##STATUS 1", "##STATUS 0", file_path);
    remove_client(cur_cli, sd);
}

void parse_user_data_login(char **usr, client *cur_cli)
{
    for (int i = 0; usr[i] != NULL; i++) {
        char **user_parsed = my_str_to_word_array(usr[i]);
        if (strcmp(user_parsed[0], "##USER") == 0) {
            cur_cli->username = strdup(user_parsed[1]);
            usr[i] && usr[i + 1] ? i++ : 0;
            continue;
        }
        if (strcmp(user_parsed[0], "##UUID") == 0) {
            cur_cli->uuid_text = strdup(user_parsed[1]);
            usr[i] && usr[i + 1] ? i++ : 0;
            continue;
        }
    }
    cur_cli->already_subscribed = true;
    char *file_path = malloc(strlen(cur_cli->uuid_text) + 13);
    strcpy(file_path, "data/users/");
    strcat(file_path, cur_cli->uuid_text);
    replace_line_file("##STATUS 0", "##STATUS 1", file_path);
}

void execute_function_login(server **serv, client *current_client, int i)
{
    void *function;
    if (i == 0) {
        function = load_library_function((*serv)->lib,
        "server_event_user_created");
        ((void (*)(char const *, char const *))function)
        (current_client->uuid_text, current_client->username);
    } else if (i == 1) {
        function = load_library_function((*serv)->lib,
        "server_event_user_logged_in");
        ((void (*)(char *))function)(current_client->uuid_text);
    }
}

// ? free(file);
// printf("Send to client : |%s|\n", str);
void login_handler(server **serv, client *cli, int sd)
{
    char **usr, str[1024];
    char *file = malloc(sizeof(char) * 1024); strcpy(file, "data/users/");
    if (!args_check((*serv)->command, 2, sd) || user_connected(cli))
        return;
    if ((usr = read_folder_files(file, (*serv)->command[1])) != NULL) {
        parse_user_data_login(usr, cli);
        sprintf(str, "%s\n%s\n%s\n", CODE_201, cli->uuid_text, cli->username);
    } else {
        cli->username = strdup((*serv)->command[1]);
        uuid_generate(cli->uuid);
        uuid_unparse(cli->uuid, cli->uuid_text);
        execute_function_login(serv, cli, 0);
        sprintf(str, "##UUID %s\n##USER %s\n##STATUS 1\n", cli->uuid_text,
        cli->username);
        write_in_file(strcat(file, cli->uuid_text), str);
        sprintf(str, "%s\n%s\n%s\n", CODE_201, cli->uuid_text, cli->username);
    }
    cli->is_logged = true;
    execute_function_login(serv, cli, 1);
    send(sd, str, strlen(str), 0);
}
