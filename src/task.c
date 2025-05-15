#include <stdio.h>

#include "task.h"

ToDoList to_do_list;

ToDoProjects to_do_projects;

void print_task(Task *task){
    if (task == NULL) {
        printf("Task is NULL\n");
        return;
    }
    printf("Task ID: %d\n", task->id);
    printf("Name: %s\n", task->name ? task->name : "(none)");
    printf("Description: %s\n", task->description ? task->description : "(none)");
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
    switch(task->recurrent){
        case -1:
            break;
        default:
            printf("Due: %d\n", task->due);

    }
    printf("Recurrent: %d\n", task->recurrent);
    printf("Status: %d\n", task->status);
    printf("Category: %s\n", task->category ? task->category : "(none)");
    printf("Project: %s\n", task->project ? task->project : "(none)");
}
