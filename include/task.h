#ifndef TASK_H
#define TASK_H

#include <stdlib.h>

#define NAME_CHARS 21
#define DESC_CHARS 201

enum Status {
    TODO,
    IN_PROGRESS,
    DONE
};

enum Priority {
    LOW,
    MEDIUM,
    HIGH,
    URGENT
};

enum Recurrent{
    NO,
    DAILY,
    WEEKLY,
    MONTHLY,
    YEARLY
};

#define append(da, element)\
    do{\
        if(da.n_items >= da.size){\
            if(da.size == 0) da.size = 256;\
            else da.size*=2;\
            da.items = realloc(da.items, da.size*sizeof(*da.items));\
        }\
        da.items[da.n_items++] = element;\
    } while(0)\

typedef struct{
    int id;
    char *name;
    char *description;
    int priority;
    int due;
    int recurrent;
    int status;
    char *category;
    char *project;

} Task;

typedef struct{
    char *name;
    Task *items;
    size_t n_items;
    size_t size;
} Project;

typedef struct{
    Task *items;
    size_t n_items;
    size_t size;
} ToDoList;

typedef struct{
    Project *items;
    size_t n_items;
    size_t size;
} ToDoProjects;

extern ToDoList to_do_list;

extern ToDoProjects to_do_projects;

void print_task(Task *task);

char *get_priority(int priority);

int get_priority_int(char *priority);

char *get_recurrence(int recurrence);

int get_recurrence_int(char *recurrence);

char *get_status(int status);

int get_status_int(char *status);

#endif