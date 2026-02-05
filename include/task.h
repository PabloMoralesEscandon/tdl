#ifndef TASK_H
#define TASK_H

#include <stdlib.h>
#include <time.h>

#define NAME_CHARS 21
#define LONG_NAME_CHARS 41
#define DESC_CHARS 201

#define ANSI_RESET     "\x1b[0m"
#define ANSI_BOLD      "\x1b[1m"
#define ANSI_BLUE      "\x1b[34m"
#define ANSI_BOLD_BLUE "\x1b[1;34m"

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

enum Due{
    LATER,
    YEAR,
    MONTH,
    WEEK,
    DAY
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
    time_t due;
    int recurrent;
    int status;
    char *category;
    char *project;
    int value;
} Task;

typedef struct{
    Task *items;
    size_t n_items;
    size_t size;
} ToDoList;

typedef struct{
    char **items;
    size_t n_items;
    size_t size;
} ToDoProjects;


extern ToDoList to_do_list;

extern ToDoProjects to_do_proj;

void print_task(Task *task);

char *get_priority(int priority);

int get_priority_int(char *priority);

char *get_recurrence(int recurrence);

int get_recurrence_int(char *recurrence);

char *get_status(int status);

int get_status_int(char *status);

int is_valid_date(struct tm date);

double second_until(time_t target);

int when_due(time_t target);

int is_in_proj_list(char *name);

void print_proj(int id); 

void print_task_table_header();

void print_task_table_row(Task *t);

void print_proj_table_header();

void print_proj_table_row(char *proj, int id);

#endif
