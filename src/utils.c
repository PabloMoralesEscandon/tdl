#include "utils.h"

#include "task.h"

void sort_list(Task *disordered, int low, int high){
    if(low < high){
        int separator = partition(disordered, low, high);
        sort_list(disordered, low, separator);
        sort_list(disordered, separator+1, high);
    }
}

int partition(Task *disordered, int low, int high){
    int i,j;
    i = low - 1;
    j= high + 1;
    int pivot = disordered[low].id;
    while(1){
        do {
            i++;
        } while(disordered[i].id < pivot);
        do {
            j--;
        } while(disordered[j].id > pivot);
        if(i >= j) return j;
        Task temp = disordered[i];
        disordered[i] = disordered[j];
        disordered[j] = temp;
    }
}