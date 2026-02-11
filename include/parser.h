#ifndef PARSER_H
#define PARSER_H

#define NUMBER_OPT 8

#define ESC "\x1b["
#define RESET ESC "0m"

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
    const char *desc;
} Command;

int dispatch_command(char *cmd, char* options[], int id);

char *parse_words(int argc, char **argv);

int parse_id_name(char *words);

void print_help(void);

int parse_options(int argc, char **argv, char* options[]);

int cmd_add(char *options[], int id);

int cmd_del(char *options[], int id);

int cmd_mod(char *options[], int id);

int cmd_start(char *options[], int id);

int cmd_done(char *options[], int id);

int cmd_show(char *options[], int id);

int cmd_list(char *options[], int id);

int cmd_list_projects(char *options[], int id);

int cmd_proj_show(char *options[], int id);

int cmd_clear(char *options[], int id);


static inline void set_bg256(int n);
static inline void set_fg256(int n);
static inline void term_bold_on(void);
static inline void term_bold_off(void);

#endif
