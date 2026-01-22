#include <stdio.h>
#include <string.h>

#include "task.h"

ToDoList to_do_list;

ToDoProjects to_do_proj;

void print_task(Task *task){
    if (task == NULL) {
        printf("Task is NULL\n");
        return;
    }
    printf("Task ID: %d\n", task->id);
    printf("Name: %s\n", task->name);
    if(strcmp("none", task->description))
        printf("Description: %s\n", task->description);
    printf("Priority: %s.\n", get_priority(task->priority));    
    struct tm *tm_info = localtime(&(task->due));
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", tm_info);
    printf("Due: %s\n", buffer);
    if(task->recurrent!=NO) printf("%s.\n", get_recurrence(task->recurrent));
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
	default:
	    return "?";
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
	default:
	    return "?";

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
	default:
	    return "?";
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

int is_leap_year(int year){
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int is_valid_date(struct tm date){
    int day = date.tm_mday;
    int month = date.tm_mon + 1;
    int year = date.tm_year + 1900;
    if (year < 1) return 0;
    if (month < 1 || month > 12) return 0;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) daysInMonth[1] = 29;
    return (day >= 1 && day <= daysInMonth[month - 1]);
}


double second_until(time_t target){
    time_t now = time(NULL);
    return difftime(target, now);  // target - now, in seconds
}

int when_due(time_t target){
    if(target == 0) return LATER;
    double seconds = second_until(target);
    if(seconds<(60*60*24)) return DAY;
    if(seconds<(60*60*24*7)) return WEEK;
    time_t now = time(NULL);
    struct tm time_now = *localtime(&now);
    time_now.tm_mon+=1;
    time_t time_1_mon = mktime(&time_now);
    if((target < time_1_mon) && (target > now)) return MONTH;
    time_now.tm_mon-=1;
    time_now.tm_year+=1;
    time_t time_1_year = mktime(&time_now);
    if((target < time_1_year) && (target > now)) return YEAR;
    return LATER;
}

int is_in_proj_list(char *name){
    for(int i=0; i<to_do_proj.n_items; i++){
	if(!strcmp(name, to_do_proj.items[i])) return 1;
    }
    return 0;
}

void print_proj(int id){
    char *project = to_do_proj.items[id];
    float tasks = 0;
    float done = 0;
    for(int i=0; i<to_do_list.n_items; i++){
	if(!strcmp(project, to_do_list.items[i].project)){
	    tasks++;
	    if(to_do_list.items[i].status == DONE) done++;
	}
    }
    printf("Project %s has %d tasks.\n", project, (int)tasks);
    int width = 20;
    float percent = done / tasks; 
    int progress = (int)(percent * width);
    printf("Project is %.2f%% done.\n", percent*100.0);
    for(int i=0; i<width; i++){
	if(i<progress){
	   printf("#");
	} else printf("-");
    }
    printf("\n\n");

    
}

void print_task_table_header() {
    printf("%-5s %-25s %-10s %-12s %-10s %-15s %-25s %-25s\n",
           "ID", "Name", "Priority", "Due", "Recurrent", "Status", "Category", "Project");
}

// Move to task.c
void print_task_table_row(Task *t) {
    struct tm *tm_info = localtime(&(t->due));
    char buffer[11];
    if(!(t->due)){ 	
	snprintf(buffer,sizeof buffer, "-");
    }else {
	strftime(buffer, sizeof(buffer), "%d-%m-%Y", tm_info);
    }
    if(!strcmp(t->category, "none")){
	free(t->category);
	(t->project) = strdup("-");
    }
    if(!strcmp(t->project, "none")){
	free(t->project);
	(t->project) = strdup("-");
    }
    printf("%-5d %-25s %-10s %-12s %-10s %-15s %-25s %-25s\n",
           t->id,
           t->name ? t->name : "(none)",
           get_priority(t->priority),
           buffer,
           get_recurrence(t->recurrent),
           get_status(t->status),
           t->category ? t->category : "-",
           t->project ? t->project : "-");
}

void print_proj_table_header() {
    printf("%-5s %-25s %10s\n",
           "ID", "Project", "Status");
}

// Move to task.c
void print_proj_table_row(char *proj, int id) {
    float tasks = 0;
    float done = 0;
    for(int i=0; i<to_do_list.n_items; i++){
	if(!strcmp(to_do_list.items[i].project, proj)){
	    tasks++;
	    if(to_do_list.items[i].status == DONE) done++;
	}
    }
    float percent = 100 * done / tasks;
    printf("%-5d %-25s %9.2f%%\n",
	id, proj, percent);
}

