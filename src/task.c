#include <stdio.h>
#include <string.h>

#include "task.h"

ToDoList to_do_list;

ToDoProjects to_do_projects;

void print_task(Task *task){
    if (task == NULL) {
        printf("Task is NULL\n");
        return;
    }
    printf("Task ID: %d\n", task->id);
    printf("Name: %s\n", task->name);
    if(strcmp("none", task->description))
        printf("Description: %s\n", task->description);
    printf("Priority: %d\n", task->priority);
    printf("%s.\n", get_priority(task->priority));
    if(task->due!=-1) printf("Due: %d\n", task->due);
    printf("%s.\n", get_recurrence(task->recurrent));
    printf("%s.\n", get_priority(task->status));
    printf("Category: %s\n", task->category ? task->category : "(none)");
    printf("Project: %s\n", task->project ? task->project : "(none)");
}

char *get_priority(int priority){
    switch(priority){
        case LOW:
            return "Low";
        case MEDIUM:
            return "Medium";
        case HIGH:
            return "High";
        case URGENT:
            return "URGENT";

    }
}

int get_priority_int(char *priority){
    if(!strcmp(priority, "low")) return LOW;
        else if(!strcmp(priority, "medium")) return MEDIUM;
        else if(!strcmp(priority, "high")) return HIGH;
        else if(!strcmp(priority, "urgent")) return URGENT;
        else{
            return -1;
        }
}

char *get_recurrence(int recurrence){
    switch(recurrence){
        case NO:
            return "No";
        case DAILY:
            return "Daily";
        case WEEKLY:
            return "Weekly";
        case MONTHLY:
            return "Monthly";
        case YEARLY:
            return "Yearly";

    }
}

int get_recurrence_int(char *recurrence){
    if(!strcmp(recurrence, "daily")) return DAILY;
        else if(!strcmp(recurrence, "weekly")) return WEEKLY;
        else if(!strcmp(recurrence, "monthly")) return MONTHLY;
        else if(!strcmp(recurrence, "yearly")) return YEARLY;
        else{
            return -1;
        }
}

char *get_status(int status){
    switch(status){
        case TODO:
            return "To do";
        case IN_PROGRESS:
            return "In progress";
        case DONE:
            return "Done";
    }
}

int get_status_int(char *status){
    if(!strcmp(status, "To do")) return TODO;
        else if(!strcmp(status, "In progress")) return IN_PROGRESS;
        else if(!strcmp(status, "Done")) return DONE;
        else{
            return -1;
        }
}