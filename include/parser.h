#ifndef PARSER_H
#define PARSER_H

#define NUMBER_OPT 8

#define PRIORITY    0
#define RECURRENT   1
#define DUE         2
#define STATUS      3
#define CATEGORY    4
#define PROJECT     5
#define NAME        6
#define DESC        7

#define FILE_NAME "tasks.json"

typedef int (*CommandHandler)(char *options[], int id);

typedef struct {
    const char *name;
    CommandHandler handler;
} Command;

int dispatch_command(char *cmd, char* options[], int id);

char *parse_words(int argc, char **argv);

int parse_id_name(char *words);

int parse_options(int argc, char **argv, char* options[]);

int cmd_add(char *options[], int id);

int cmd_del(char *options[], int id);

int cmd_mod(char *options[], int id);

int cmd_start(char *options[], int id);

int cmd_done(char *options[], int id);

int cmd_show(char *options[], int id);

int cmd_list(char *options[], int id);

#endif