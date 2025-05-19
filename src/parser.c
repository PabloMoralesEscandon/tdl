#include "parser.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "task.h"
#include "memory.h"

Command commands[] = {
    {"add",    cmd_add},
    {"show",    cmd_show},
    {NULL,     NULL}  // Sentinel to mark end
};

int dispatch_command(char *cmd, char* options[], int id) {
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(commands[i].name, cmd) == 0) {
            return commands[i].handler(options, id);
        }
    }
    printf("Unknown command: %s\n", cmd);
    return 1;
}

char *parse_words(int argc, char **argv){
    int i = 2;
    while((i<argc)&&(argv[i][0]!='-')) i++;
    int size = 0;
    for(int n=2; n<i; n++){
        size+=strlen(argv[n])+1;
    }
    if(size>NAME_CHARS){
        printf("Name is too long\n");
        return 0;
    }
    char *words=malloc(size);
    if(i>2) strcpy(words, argv[2]);
    for(int n=3; n<i; n++){
        strcat(words, " ");
        strcat(words, argv[n]);
    }
    printf("%d  %s\n", i, words);
    return words;
}

int parse_id_name(char *words){
    int i=0;
    while(isdigit(words[i++]));
    if((i-1)==strlen(words)){
        printf("It is an id\n");
        return atoi(words);
    } else{
        printf("It is a name\n");
        return -1;
    }
}

int parse_options(int argc, char **argv, char **options){
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

    return 0;
}

int cmd_add(char *options[], int id){
    Task new_task = {0};
    int task_id = 0;
    while(to_do_list.items[task_id].id == task_id) task_id++;
    new_task.id = task_id;
    new_task.status = TODO;
    if (options[NAME]) {
        if ((strlen(options[NAME])+1) > NAME_CHARS) {
            printf("Name is too long. Max %d characters.\n", NAME_CHARS);
            return 1;
        }
        new_task.name = strdup(options[NAME]);
    }
    else {
        char name[NAME_CHARS];
        snprintf(name, sizeof(name), "Task %d", new_task.id);
        new_task.name = strdup(name);
    }
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
    } else new_task.project = "none";
    if(options[CATEGORY]){
        new_task.category = options[CATEGORY];
    } else new_task.category = "none";
    if(options[DESC]){
        new_task.description = options[DESC];
    } else new_task.description = "none";
    append(to_do_list, new_task);
    save(&new_task, FILE_NAME);
    return 0;
}

int cmd_del(char *options[], int id){
    printf("Success!\n");
    return 0;
}

int cmd_list(char *options[], int id){
    printf("Success!\n");
    return 0;
}

int cmd_mod(char *options[], int id){
    printf("Success!\n");
    return 0;
}

int cmd_show(char *options[], int id){
    if(id!=-1){
        for(int i=0; i<to_do_list.n_items; i++){
            if(id == to_do_list.items[i].id){
                print_task(&to_do_list.items[i]);
                return(0);
            }
        }

    }
    for(int i=0; i<to_do_list.n_items; i++){
        if(!strcmp(options[NAME], to_do_list.items[i].name)){
            print_task(&to_do_list.items[i]);
            return(0);
        }
    }
    printf("Could not find task: %s", options[NAME]);
    return 1;
}