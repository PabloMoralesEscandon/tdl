#ifndef UTILS_H
#define UTILS_H

#include "task.h"

void sort_list(Task *disordered, int low, int high);

int partition(Task *disordered, int low, int high);

#endif