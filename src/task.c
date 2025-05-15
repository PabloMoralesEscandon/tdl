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
