#include "parser.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "task.h"
#include "memory.h"
#include "utils.h"

Command commands[] = {
    {"add",    		cmd_add,		"Add a new task"},
    {"show",    	cmd_show,		"Show information of a task"},
    {"mod",    		cmd_mod,		"Modify an existing task"},
    {"start",   	cmd_start,		"Mark a task as started"},
    {"done",    	cmd_done,		"Mark a task as done"},
    {"del",    		cmd_del,		"Delete a task"},
    {"list",    	cmd_list,		"List all tasks"},
    {"list_projects", 	cmd_list_projects,	"List all projects"},
    {"show_project", 	cmd_proj_show,		"Show information of a project"},
    {NULL,     NULL}  // Sentinel to mark end
};

int dispatch_command(char *cmd, char* options[], int id) {
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(commands[i].name, cmd) == 0) {
            return commands[i].handler(options, id);
        }
    }
    printf("Unknown command: %s\n", cmd);
    return 1;
}

char *parse_words(int argc, char **argv){
    int i = 2;
    while((i<argc)&&(argv[i][0]!='-')) i++;
    int size = 0;
    for(int n=2; n<i; n++){
        size+=strlen(argv[n])+1;
    }
    if(!size) return NULL;
    char *words=malloc(size);
    if(i>2) strcpy(words, argv[2]);
    for(int n=3; n<i; n++){
        strcat(words, " ");
        strcat(words, argv[n]);
    }
    printf("%d  %s\n", i, words);
    return words;
}

int parse_id_name(char *words){
    int i=0;
    while(isdigit(words[i++]));
    if((i-1)==strlen(words)){
        printf("It is an id\n");
        return atoi(words);
    } else{
        printf("It is a name\n");
        return -1;
    }
}

void print_help(void){
    puts("TDL (ToDoList): A command line to do list.");
    puts("Usage: tdl [command] [id or name] <options>");
    puts("");
    puts("Commands:");
    for(int i=0; commands[i].name != NULL; i++) printf("  %s: %s\n", commands[i].name, commands[i].desc);
    puts("");
    puts("Options:");
    puts("  -p, --priority <val>       Task priority");
    puts("  -r, --recurrent <val>      Recurrent rule / interval");
    puts("  -d, --due <val>            Due date/time");
    puts("  -s, --status <val>         Task status");
    puts("  -c, --category <val>       Category");
    puts("  -P, --project <val>        Project");
    puts("  -n, --name <val>           Task name/title");
    puts("  -D, --description <val>    Task description");
    puts("  -h, --help                 Show this help and exit");
}

int parse_options(int argc, char **argv, char **options){
    static struct option long_options[] = {
        {"priority",    required_argument, 0, 'p'},
        {"recurrent",   required_argument, 0, 'r'},
        {"due",         required_argument, 0, 'd'},
        {"status",      required_argument, 0, 's'},
        {"category",    required_argument, 0, 'c'},
        {"project",     required_argument, 0, 'P'},
        {"name",        required_argument, 0, 'n'},
        {"description", required_argument, 0, 'D'},
	{"help", 	no_argument, 	   0, 'h'},
        {0, 0, 0, 0}  
    };

    
    int opt;

    while ((opt = getopt_long(argc, argv, "p:r:d:s:c:P:n:D:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                options[PRIORITY] = strdup(optarg);
                break;
            case 'r':
                options[RECURRENT] = strdup(optarg);
                break;
            case 'd':
                options[DUE] = strdup(optarg);
                break;
            case 's':
                options[STATUS] = strdup(optarg);
                break;
            case 'c':
                options[CATEGORY] = strdup(optarg);
                break;
            case 'P':
                options[PROJECT] = strdup(optarg);
                break;
            case 'n':
                options[NAME] = strdup(optarg);
                break;
            case 'D':
                options[DESC] = strdup(optarg);
                break;
	    case 'h':
		print_help();
		exit(0);
            default:
                printf("Unknown option:\n");
        }
    }

    return 0;
}

int cmd_add(char *options[], int id){
    Task new_task = {0};
    int task_id;
    new_task.id = -1;
    for(task_id=0; task_id<to_do_list.n_items; task_id++){
        if(task_id!=to_do_list.items[task_id].id) break;
    }
    new_task.id = task_id;
    new_task.status = TODO;
    if (options[NAME]) {
        if ((strlen(options[NAME])+1) > NAME_CHARS) {
            printf("Name is too long. Max %d characters.\n", NAME_CHARS);
            return 1;
        }
        new_task.name = strdup(options[NAME]);
    }
    else {
        char name[NAME_CHARS];
        snprintf(name, sizeof(name), "Task %d", new_task.id);
        new_task.name = strdup(name);
    }
    if(options[PRIORITY]){
        if(!strcmp(options[PRIORITY], "low")) new_task.priority = LOW;
        else if(!strcmp(options[PRIORITY], "medium")) new_task.priority = MEDIUM;
        else if(!strcmp(options[PRIORITY], "high")) new_task.priority = HIGH;
        else if(!strcmp(options[PRIORITY], "urgent")) new_task.priority = URGENT;
        else{
            printf("Invalid priority option: %s\nValid options: low, medium, high, urgent\n", options[RECURRENT]);
            return 1;
        }
    } else new_task.priority = MEDIUM;
    if(options[RECURRENT]){
        if(!strcmp(options[RECURRENT], "daily")) new_task.recurrent = DAILY;
        else if(!strcmp(options[RECURRENT], "weekly")) new_task.recurrent = WEEKLY;
        else if(!strcmp(options[RECURRENT], "monthly")) new_task.recurrent = MONTHLY;
        else if(!strcmp(options[RECURRENT], "yearly")) new_task.recurrent = YEARLY;
        else{
            printf("Invalid recurrence option: %s\nValid options: daily, weekly, monthly, yearly\n", options[RECURRENT]);
            return 1;
        }
    } else new_task.recurrent = NO;
    if(options[DUE]){
        struct tm date = {0};
        if( (strlen(options[DUE]) == 10)
            && (isdigit(options[DUE][0]) && isdigit(options[DUE][1]))
            && (isdigit(options[DUE][3]) && isdigit(options[DUE][4]))
            && (isdigit(options[DUE][6]) && isdigit(options[DUE][7]))
            && (isdigit(options[DUE][8]) && isdigit(options[DUE][9]))
            && ((options[DUE][2] == options[DUE][5]) && (options[DUE][2] == '-'))
        ){
            date.tm_mday = (options[DUE][0] - '0') * 10 + (options[DUE][1] - '0');
            date.tm_mon = (options[DUE][3] - '0') * 10 + (options[DUE][4] - '0')-1;
            date.tm_year = (options[DUE][8] - '0') * 10 + (options[DUE][9] - '0');
            date.tm_year += (options[DUE][6] - '0') * 1000 + (options[DUE][7] - '0') * 100;
	    date.tm_year-=1900;
	} else{
            printf("Failed to parse date or invalid format. Make sure date is in format DD-MM-YYYY.\n");
            return 0;
        }
        if(is_valid_date(date)){
            new_task.due = mktime(&date);
        } else{
            printf("Failed to parse date or invalid format. Make sure date is in format DD-MM-YYYY.\n");
            return 0;
	}
    } else new_task.due = 0;
    if(options[PROJECT]){
	if ((strlen(options[PROJECT])+1) > NAME_CHARS) {
            printf("Project is too long. Max %d characters.\n", NAME_CHARS);
            return 1;
        }
	new_task.project = strdup(options[PROJECT]);
    } else new_task.project = "none";
    if(options[CATEGORY]){
	if ((strlen(options[CATEGORY])+1) > NAME_CHARS) {
            printf("Category is too long. Max %d characters.\n", NAME_CHARS);
            return 1;
        }
        new_task.category = options[CATEGORY];
    } else new_task.category = "none";
    if(options[DESC]){
        if ((strlen(options[DESC])+1) > DESC_CHARS) {
            printf("Description is too long. Max %d characters.\n", DESC_CHARS);
            return 1;
        }
        new_task.description = strdup(options[DESC]);
    } else new_task.description = "none";
    append(to_do_list, new_task);
    save(&new_task, FILE_NAME);
    return 0;
}

int cmd_del(char *options[], int id){
    if(id!=-1){
        delete_task(FILE_NAME, id);
        printf("Deleted task: %d\n", id);
        return 0;
    } else if(options[NAME]!=NULL){
        for(int i=0; i<to_do_list.n_items; i++){
            if(!strcmp(options[NAME], to_do_list.items[i].name)){
                id = to_do_list.items[i].id;
                delete_task(FILE_NAME, id);
                printf("Deleted task: %d\n", id);
                return 0;
            }
        }
        printf("No task with name: %s\n", options[NAME]);
        return 1;
    }
    printf("Please provide id or name of task to delete\n");
    return 1;
}

// Fix implementation (if no id -> get id -> logic)
// Same for start and done
int cmd_mod(char *options[], int id){
    int index = -1;
    if(id==-1){
        if(options[NAME]!=NULL){
            for(int i=0; i<to_do_list.n_items; i++){
                if(!strcmp(options[NAME], to_do_list.items[i].name)){
                    index = i;
                    break;
                }
            }
            if(index == -1){
                printf("Could not find task %s\n", options[NAME]);
                return 1;
            }
        } else{
            printf("Provide either an id or a name for the task.\n");
            return 1;
        }
            
    } else for(int i=0; i<to_do_list.n_items; i++){
        if(id == to_do_list.items[i].id){
            index = i;
            break;

        }
    }
    if(index == -1){
        printf("Could not find task with id %d\n", id);
        return 1;
    }
    if(options[NAME]){
        if ((strlen(options[NAME])+1) > NAME_CHARS) {
            printf("Name is too long. Max %d characters.\n", NAME_CHARS);
            return 1;
        }
	free(to_do_list.items[index].name);
        to_do_list.items[index].name = strdup(options[NAME]);
    }
    if(options[PRIORITY]){
        if(!strcmp(options[PRIORITY], "low")) to_do_list.items[index].priority = LOW;
        else if(!strcmp(options[PRIORITY], "medium")) to_do_list.items[index].priority = MEDIUM;
        else if(!strcmp(options[PRIORITY], "high")) to_do_list.items[index].priority = HIGH;
        else if(!strcmp(options[PRIORITY], "urgent")) to_do_list.items[index].priority = URGENT;
        else{
            printf("Invalid priority option: %s\nValid options: low, medium, high, urgent\n", options[RECURRENT]);
            return 1;
        }
    }
    if(options[RECURRENT]){
        if(!strcmp(options[RECURRENT], "daily")) to_do_list.items[index].recurrent = DAILY;
        else if(!strcmp(options[RECURRENT], "weekly")) to_do_list.items[index].recurrent = WEEKLY;
        else if(!strcmp(options[RECURRENT], "monthly")) to_do_list.items[index].recurrent = MONTHLY;
        else if(!strcmp(options[RECURRENT], "yearly")) to_do_list.items[index].recurrent = YEARLY;
        else{
            printf("Invalid recurrence option: %s\nValid options: daily, weekly, monthly, yearly\n", options[RECURRENT]);
            return 1;
        }
    }
    if(options[DUE]){
        struct tm date = {0};
        if( (strlen(options[DUE]) == 10)
            && (isdigit(options[DUE][0]) && isdigit(options[DUE][1]))
            && (isdigit(options[DUE][3]) && isdigit(options[DUE][4]))
            && (isdigit(options[DUE][6]) && isdigit(options[DUE][7]))
            && (isdigit(options[DUE][8]) && isdigit(options[DUE][9]))
            && ((options[DUE][2] == options[DUE][5]) && (options[DUE][2] == '-'))
        ){
            date.tm_mday = (options[DUE][0] - '0') * 10 + (options[DUE][1] - '0');
            date.tm_mon = (options[DUE][3] - '0') * 10 + (options[DUE][4] - '0')-1;
            date.tm_year = (options[DUE][8] - '0') * 10 + (options[DUE][9] - '0');
            date.tm_year += (options[DUE][6] - '0') * 1000 + (options[DUE][7] - '0') * 100;
	    date.tm_year-=1900;
	} else{
            printf("Failed to parse date or invalid format. Make sure date is in format DD-MM-YYYY.\n");
            return 0;
        }
        if(is_valid_date(date)){
            to_do_list.items[index].due = mktime(&date);
        } else{
            printf("Failed to parse date or invalid format. Make sure date is in format DD-MM-YYYY.\n");
            return 0;
	}
	
    }
    if(options[PROJECT]){
	if ((strlen(options[PROJECT])+1) > NAME_CHARS) {
            printf("Project is too long. Max %d characters.\n", NAME_CHARS);
            return 1;
        }
	free(to_do_list.items[index].project);
	to_do_list.items[index].project = strdup(options[PROJECT]);
    }
    if(options[CATEGORY]){
	if ((strlen(options[CATEGORY])+1) > NAME_CHARS) {
            printf("Category is too long. Max %d characters.\n", NAME_CHARS);
            return 1;
        }
        free(to_do_list.items[index].category);
        to_do_list.items[index].category = strdup(options[CATEGORY]);
    }
    if(options[DESC]){
        if ((strlen(options[DESC])+1) > DESC_CHARS) {
            printf("Description is too long. Max %d characters.\n", DESC_CHARS);
            return 1;
        }
        free(to_do_list.items[index].description);
        to_do_list.items[index].description = strdup(options[DESC]);
    }
    id = to_do_list.items[index].id;
    delete_task(FILE_NAME, id);
    save(&to_do_list.items[index], FILE_NAME);
    return 0;
}

int cmd_start(char *options[], int id){
    if(id!=-1){
        for(int i=0; (i<to_do_list.n_items) && (i<=id); i++){
            if(id==to_do_list.items[i].id){
                to_do_list.items[i].status = IN_PROGRESS;
                break;
            }
        }
    } else if(options[NAME]!=NULL){
        for(int i=0; i<to_do_list.n_items; i++){
            if(!strcmp(options[NAME], to_do_list.items[i].name)){
                to_do_list.items[i].status = IN_PROGRESS;
                id = to_do_list.items[i].id;
                break;
            }
        }
    }
    delete_task(FILE_NAME, id);
    save(&to_do_list.items[id], FILE_NAME);
    return 0;
}

int cmd_done(char *options[], int id){
    int index = -1;
    if(id!=-1){
        for(int i=0; i<to_do_list.n_items; i++){
            if(id==to_do_list.items[i].id){
                to_do_list.items[i].status = DONE;
                index =i;
		break;
            }
        }
    } else if(options[NAME]!=NULL){
        for(int i=0; i<to_do_list.n_items; i++){
            if(!strcmp(options[NAME], to_do_list.items[i].name)){
                to_do_list.items[i].status = DONE;
                id = to_do_list.items[i].id;
                index = i;
		break;
            }
        }
    } else{
	printf("Provide anme or id ot he completed task.\n");
	return 1;
    }
    if(index==-1){
	printf("Could not find the completed task");
	return 1;
    }
    delete_task(FILE_NAME, id);
    save(&to_do_list.items[index], FILE_NAME);
    return 0;
}

int cmd_show(char *options[], int id){
    if(id!=-1){
        for(int i=0; i<to_do_list.n_items; i++){
            if(id == to_do_list.items[i].id){
                print_task(&to_do_list.items[i]);
                return(0);
            }
        }
	printf("No task with id %d\n", id);
	return 1;

    }
    for(int i=0; i<to_do_list.n_items; i++){
        if(!strcmp(options[NAME], to_do_list.items[i].name)){
            print_task(&to_do_list.items[i]);
            return(0);
        }
    }
    printf("Could not find task: %s", options[NAME]);
    return 1;
}

int cmd_list(char *options[], int id) {
    sort_list_value(to_do_list.items, 0, to_do_list.n_items-1); 
    set_bg256(230);
    set_fg256(232);
    term_bold_on();
    print_task_table_header();
    term_bold_off();
    printf(RESET);
    int alternate = 0;
    int done = 0;
    for (int i = 0; i < to_do_list.n_items; i++) {

        if((id!=-1) && (to_do_list.items[i].id!=id)) continue;
        if(options[PRIORITY] && (to_do_list.items[i].priority!=get_priority_int(options[PRIORITY]))) continue;
        if(options[RECURRENT] && (to_do_list.items[i].recurrent!=get_recurrence_int(options[RECURRENT]))) continue;
        if(options[DUE]){
	    if(!strcmp(options[DUE], "today")){
		if(when_due(to_do_list.items[i].due) != DAY) continue;
	    }
	    if(!strcmp(options[DUE], "week")){
		if(when_due(to_do_list.items[i].due) < WEEK) continue;
	    }
	    if(!strcmp(options[DUE], "month")){
		if(when_due(to_do_list.items[i].due) < MONTH) continue;
	    }
	    if(!strcmp(options[DUE], "year")){
		if(when_due(to_do_list.items[i].due) < YEAR) continue;
	    }
	}
	if((options[STATUS] && strcmp(options[STATUS], "all")) && (options[STATUS] && (to_do_list.items[i].status!=get_status_int(options[STATUS]))) || (!options[STATUS] && (to_do_list.items[i].status==DONE))) continue;
        if(options[CATEGORY] && (strcmp(to_do_list.items[i].category, options[CATEGORY]))) continue;
        if(options[PROJECT] && (strcmp(to_do_list.items[i].project,options[PROJECT]))) continue;
        if(options[NAME] && (strcmp(to_do_list.items[i].name, options[NAME]))) continue;
        if(options[DESC] && (strcmp(to_do_list.items[i].description, options[DESC]))) continue;
	int bg;
	if(alternate){
	    bg = 235;
	}else bg = 237;
	alternate = !alternate;
	set_bg256(bg);
        print_task_table_row(&to_do_list.items[i]);
	printf(RESET);
    }
    return 0;
}

int cmd_list_projects(char *options[], int id){
    set_bg256(230);
    set_fg256(232);
    term_bold_on();
    print_proj_table_header();
    term_bold_off();
    printf(RESET);
    int alternate = 0;
    for(int i=0; i<to_do_proj.n_items; i++){
	int bg;
	if(alternate){
	    bg = 235;
	}else bg = 237;
	alternate = !alternate;
	set_bg256(bg);
	print_proj_table_row(to_do_proj.items[i], i);
	printf(RESET);
    }
    return 0;
}

int cmd_proj_show(char *options[], int id){
    if(id!=-1){ 
	if(id<to_do_proj.n_items){
	    options[PROJECT] = strdup(to_do_proj.items[id]);
	    options[STATUS] = strdup("all");
	    print_proj(id);
	    cmd_list(options, -1);
	    return 0;
	} else{
	    printf("No project with id %d.", id);
	    return 1;
	}
    } else{
	for(int i=0; i<to_do_proj.n_items; i++){
	    if(!strcmp(options[NAME], to_do_proj.items[i])){
		options[PROJECT] = strdup(to_do_proj.items[i]);
		options[STATUS] = strdup("all");
		options[NAME]=NULL;
		print_proj(i);
		cmd_list(options, -1);
		return 0;
	    }
	}
    }
    printf("No project with name %s", options[NAME]);
    return 1;
}
static void set_bg256(int n) { printf(ESC "48;5;%dm", n); }
static void set_fg256(int n) { printf(ESC "38;5;%dm", n); }
static inline void term_bold_on(void)  { fputs("\x1b[1m", stdout); }   // bold on [web:14]
static inline void term_bold_off(void) { fputs("\x1b[22m", stdout); }
