#include <stdio.h>
#include <string.h>

#include "task.h"

ToDoList to_do_list;

ToDoProjects to_do_proj;


static const char *dash_if_missing(const char *s) {
    // Treat NULL, empty string, or literal "none" as missing. [web:7][web:10]
    if (s == NULL || s[0] == '\0' || strcmp(s, "none") == 0) return "-";
    return s;
}

static void format_due_date(char out[11], time_t due) {
    if (due == (time_t)0) {                      // "unset" convention
        snprintf(out, 11, "-");
        return;
    }
    struct tm *tm_info = localtime(&due);        // convert to local time [web:15]
    if (!tm_info) {                              // localtime can fail [web:6]
        snprintf(out, 11, "-");
        return;
    }
    strftime(out, 11, "%d-%m-%Y", tm_info);
}

static void print_label_value(const char *label, const char *value) {
    printf(ANSI_BOLD_BLUE "%s" ANSI_RESET " %s\n", label, value); // style via ANSI [web:5][web:2]
}

void print_task(Task *task) {
    if (!task) {
        printf("Task is NULL\n");
        return;
    }

    char due_buf[11];
    format_due_date(due_buf, task->due);

    // Labels bold+blue, values normal.
    printf(ANSI_BOLD_BLUE "Task ID:" ANSI_RESET " %d\n", task->id);                          // ANSI [web:5][web:2]
    print_label_value("Name:", task->name ? task->name : "-");

    if (task->description && task->description[0] != '\0' && strcmp(task->description, "none") != 0) {
        print_label_value("Description:", task->description);
    }

    print_label_value("Priority:", get_priority(task->priority));
    print_label_value("Due:", due_buf);

    if (task->recurrent != NO) {
        print_label_value("Recurrence:", get_recurrence(task->recurrent));
    }

    print_label_value("Category:", dash_if_missing(task->category));
    print_label_value("Project:",  dash_if_missing(task->project));
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
    if(!strcmp(status, "to do")) return TODO;
    else if(!strcmp(status, "In progress")) return IN_PROGRESS;
    else if(!strcmp(status, "in progress")) return IN_PROGRESS;
    else if(!strcmp(status, "todo")) return TODO;
    else if(!strcmp(status, "ongoing")) return IN_PROGRESS;
    else if(!strcmp(status, "Done")) return DONE;
    else if(!strcmp(status, "done")) return DONE;
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
    for(size_t i=0; i<to_do_proj.n_items; i++){
	if(!strcmp(name, to_do_proj.items[i])) return 1;
    }
    return 0;
}

void print_proj(int id){
    char *project = to_do_proj.items[id];
    float tasks = 0;
    float done = 0;
    for(size_t i=0; i<to_do_list.n_items; i++){
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

void print_proj_table_row(char *proj, int id) {
    float tasks = 0;
    float done = 0;
    for(size_t i=0; i<to_do_list.n_items; i++){
	if(!strcmp(to_do_list.items[i].project, proj)){
	    tasks++;
	    if(to_do_list.items[i].status == DONE) done++;
	}
    }
    float percent = 100 * done / tasks;
    printf("%-5d %-25s %9.2f%%\n",
	id, proj, percent);
}

