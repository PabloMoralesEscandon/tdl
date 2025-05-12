#include "parser.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "memory.h"

Command commands[] = {
    {"add",    cmd_add},
    {NULL,     NULL}  // Sentinel to mark end
};

int dispatch_command(char *cmd, char* options[]) {
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(commands[i].name, cmd) == 0) {
            return commands[i].handler(options);
        }
    }
    printf("Unknown command: %s\n", cmd);
    return 1;
}

char **parse_options(int argc, char **argv, char **options){
    static struct option long_options[] = {
        {"priority",    required_argument, 0, 'p'},
        {"recurrent",   required_argument, 0, 'r'},
        {"due",         required_argument, 0, 'd'},
        {"status",      required_argument, 0, 's'},
        {"category",    required_argument, 0, 'c'},
        {"project",     required_argument, 0, 'P'},
        {"name",        required_argument, 0, 'n'},
        {"description", required_argument, 0, 'D'},
        {0, 0, 0, 0}  
    };

    
    int opt;

    while ((opt = getopt_long(argc, argv, "p:r:d:s:c:P:n:D:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                options[PRIORITY] = strdup(optarg);
                break;
            case 'r':
                options[RECURRENT] = strdup(optarg);
                break;
            case 'd':
                options[DUE] = strdup(optarg);
                break;
            case 's':
                options[STATUS] = strdup(optarg);
                break;
            case 'c':
                options[CATEGORY] = strdup(optarg);
                break;
            case 'P':
                options[PROJECT] = strdup(optarg);
                break;
            case 'n':
                options[NAME] = strdup(optarg);
                break;
            case 'D':
                options[DESC] = strdup(optarg);
                break;
            default:
                printf("Unknown option:\n");
        }
    }

    return options;
}

int cmd_add(char *options[]){
    Task new_task = {0};
    new_task.id = 0;
    new_task.status = TODO;
    if(options[NAME]) new_task.name = strdup(options[NAME]);
    else new_task.name = strdup( "Task");
    if(options[PRIORITY]){
        // Add logic
    } else new_task.priority = MEDIUM;
    if(options[RECURRENT]){
        // Add logic
    } else new_task.recurrent = NO;
    if(options[DUE]){
        // Add logic
    } else new_task.due = -1;
    if(options[PROJECT]){
        // Add logic
    } else new_task.project = NULL;
    if(options[CATEGORY]){
        new_task.category = options[CATEGORY];
    } else new_task.category = NULL;
    if(options[DESC]){
        new_task.description = options[DESC];
    } else new_task.description = 0;
    append(to_do_list, new_task);
    save(&new_task, FILE_NAME);

    printf("Success!\n");
    return 0;
}

int cmd_del(char *options[]){
    printf("Success!\n");
    return 0;
}

int cmd_list(char *options[]){
    printf("Success!\n");
    return 0;
}

int cmd_mod(char *options[]){
    printf("Success!\n");
    return 0;
}