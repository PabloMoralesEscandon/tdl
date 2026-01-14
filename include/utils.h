#ifndef UTILS_H
#define UTILS_H

#include "task.h"

void sort_list(Task *disordered, int low, int high);

void sort_list_value(Task *disordered, int low, int high);

int partition(Task *disordered, int low, int high);

int partition_value(Task *disordered, int low, int high);

#endif
