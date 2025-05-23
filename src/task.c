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
    switch(task->priority){
        case 0:
            printf("Priority: Low\n");
            break;
        case 1:
            printf("Priority: Medium\n");
            break;
        case 2:
            printf("Priority: High\n");
            break;
        case 3:
            printf("Priority: URGENT\n");
            break;

    }
    if(task->due!=-1) printf("Due: %d\n", task->due);
    switch(task->recurrent){
        case 0:
            break;
        case 1:
            printf("Daily.\n");
            break;
        case 2:
            printf("Weekly.\n");
            break;
        case 3:
            printf("Monthly\n");
            break;
        case 4:
            printf("Yearly\n");
            break;

    }
    printf("Recurrent: %d\n", task->recurrent);
    switch(task->status){
        case TODO:
            printf("To do.\n");
            break;
        case IN_PROGRESS:
            printf("In progress.\n");
            break;
        case DONE:
            printf("Done.\n");
            break;
    }
    printf("Category: %s\n", task->category ? task->category : "(none)");
    printf("Project: %s\n", task->project ? task->project : "(none)");
}
