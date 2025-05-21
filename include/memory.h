#ifndef MEMORY_H
#define MEMORY_H

#include "task.h"

void load(ToDoList *todo_list, const char *filename);

void save(Task *task, const char *filename);

int delete_task(const char *filename, int target_id);

#endif