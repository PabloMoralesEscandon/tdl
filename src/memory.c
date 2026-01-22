#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "task.h"
#include "utils.h"

FILE *open_json_or_create_empty(const char *path){
    json_error_t error;
    json_t *root = json_load_file(path, 0, &error);  // returns NULL on error [web:90]

    if (!root) {
        // Create empty JSON array (use json_object() if you want {}). [web:48]
        json_t *empty = json_array();                // new reference, initially empty [web:48]
        if (!empty) return NULL;

        // Write JSON to file; if path exists, it is overwritten. [web:51]
        if (json_dump_file(empty, path, JSON_INDENT(2)) != 0) {
            json_decref(empty);
            return NULL;
        }
        json_decref(empty);

        // Now it exists and is valid JSON; open stream for the caller.
        return fopen(path, "r");
    }

    // File exists and parsed OK; free parsed JSON and open stream for reading.
    json_decref(root);
    return fopen(path, "r");
}

void load(const char *filename) {
    // Open the file for reading
    FILE *file = open_json_or_create_empty(filename);
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Load the JSON array from the file
    json_error_t error;
    json_t *jarray = json_loadf(file, 0, &error);
    fclose(file);
    if (jarray == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    if (!json_is_array(jarray)) {
        fprintf(stderr, "JSON root is not an array\n");
        json_decref(jarray);
        return;
    }

    size_t items_count = json_array_size(jarray);
    free(to_do_list.items);
    free(to_do_proj.items);

    // Iterate over the JSON array and fill the ToDoList
    for (size_t i = 0; i < items_count; i++) {
        json_t *json_item = json_array_get(jarray, i);
        Task new_task;
        if (!json_is_object(json_item)) {
            fprintf(stderr, "Item %zu is not a JSON object\n", i);
            continue;
        }

        json_t *json_id = json_object_get(json_item, "id");
        json_t *json_name = json_object_get(json_item, "name");
        json_t *json_desc = json_object_get(json_item, "description");
        json_t *json_prio = json_object_get(json_item, "priority");
        json_t *json_sec = json_object_get(json_item, "tm_sec");
        json_t *json_min = json_object_get(json_item, "tm_min");
        json_t *json_hour = json_object_get(json_item, "tm_hour");
        json_t *json_mday = json_object_get(json_item, "tm_mday");
        json_t *json_mon = json_object_get(json_item, "tm_mon");
        json_t *json_year = json_object_get(json_item, "tm_year");
        json_t *json_wday = json_object_get(json_item, "tm_wday");
        json_t *json_yday = json_object_get(json_item, "tm_yday");
        json_t *json_isdst = json_object_get(json_item, "tm_isdst");
        json_t *json_cat = json_object_get(json_item, "category");
        json_t *json_rec = json_object_get(json_item, "recurrent");
        json_t *json_stat = json_object_get(json_item, "status");
        json_t *json_pro = json_object_get(json_item, "project");

        if (
            json_is_integer(json_id) &&
            json_is_string(json_name) &&
            json_is_string(json_desc) &&
            json_is_integer(json_prio) &&
            json_is_integer(json_sec) &&
            json_is_integer(json_min) &&
            json_is_integer(json_hour) &&
            json_is_integer(json_mday) &&
            json_is_integer(json_mon) &&
            json_is_integer(json_year) &&
            json_is_integer(json_wday) &&
            json_is_integer(json_yday) &&
            json_is_integer(json_isdst) &&
            json_is_string(json_cat) &&
            json_is_integer(json_rec) &&
            json_is_integer(json_stat) &&
            json_is_string(json_pro)
        ) {
            new_task.id = (int)json_integer_value(json_id);
            new_task.name = strdup(json_string_value(json_name));
            new_task.description = strdup(json_string_value(json_desc));
            new_task.priority = (int)json_integer_value(json_prio);
            struct tm date = {0};
            date.tm_sec = (int)json_integer_value(json_sec);
            date.tm_min = (int)json_integer_value(json_min);
            date.tm_hour = (int)json_integer_value(json_hour);
            date.tm_mday = (int)json_integer_value(json_mday);
            date.tm_mon = (int)json_integer_value(json_mon);
            date.tm_year = (int)json_integer_value(json_year);
            date.tm_wday = (int)json_integer_value(json_wday);
            date.tm_yday = (int)json_integer_value(json_yday);
            date.tm_isdst = (int)json_integer_value(json_isdst);
            new_task.due = mktime(&date);
            new_task.category = strdup(json_string_value(json_cat));
            new_task.recurrent = (int)json_integer_value(json_rec);
            new_task.status = (int)json_integer_value(json_stat);
            new_task.project = strdup(json_string_value(json_pro));
	    new_task.value = -(new_task.priority + when_due(new_task.due));
        } else {
            fprintf(stderr, "Invalid or missing fields in item %zu\n", i);
            // Initialize with defaults or skip
            new_task.id = 0;
            new_task.name = strdup("");
            new_task.description = strdup("");
            new_task.priority = 0;
            new_task.due = 0;
            new_task.category = strdup("");
            new_task.recurrent = 0;
            new_task.status = 0;
            new_task.project = strdup("");
	    new_task.value = 0;
        }
        append(to_do_list, new_task);
	if(strcmp(new_task.project, "none") && !is_in_proj_list(new_task.project)){
	    append(to_do_proj, new_task.project);
	}
    }

    json_decref(jarray);

    sort_list(to_do_list.items, 0, to_do_list.n_items - 1);
}

void save(Task *task, const char *filename) {
    json_t *jarray = NULL;
    json_error_t error;

    // Try to open existing file for reading
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        jarray = json_loadf(file, 0, &error);
        fclose(file);
        if (jarray == NULL) {
            fprintf(stderr, "Error parsing JSON: %s\n", error.text);
            // Create new array if parsing failed
            jarray = json_array();
        } else if (!json_is_array(jarray)) {
            fprintf(stderr, "JSON root is not an array, creating new array\n");
            json_decref(jarray);
            jarray = json_array();
        }
    } else {
        // File does not exist, create new JSON array
        jarray = json_array();
    }

    // Create a new JSON object for the task
    json_t *json_task = json_object();

    // Add the task properties to the JSON object
    json_object_set_new(json_task, "id", json_integer(task->id));
    json_object_set_new(json_task, "name", json_string(task->name));
    json_object_set_new(json_task, "description", json_string(task->description));
    json_object_set_new(json_task, "priority", json_integer(task->priority));
    struct tm *date = localtime(&(task->due));
    json_object_set_new(json_task, "tm_sec",   json_integer(date->tm_sec));
    json_object_set_new(json_task, "tm_min",   json_integer(date->tm_min));
    json_object_set_new(json_task, "tm_hour",  json_integer(date->tm_hour));
    json_object_set_new(json_task, "tm_mday",   json_integer(date->tm_mday));
    json_object_set_new(json_task, "tm_mon",   json_integer(date->tm_mon));
    json_object_set_new(json_task, "tm_year",  json_integer(date->tm_year));
    json_object_set_new(json_task, "tm_wday",   json_integer(date->tm_wday));
    json_object_set_new(json_task, "tm_yday",   json_integer(date->tm_yday));
    json_object_set_new(json_task, "tm_isdst",  json_integer(date->tm_isdst));
    json_object_set_new(json_task, "category", json_string(task->category));
    json_object_set_new(json_task, "recurrent", json_integer(task->recurrent));
    json_object_set_new(json_task, "status", json_integer(task->status));
    json_object_set_new(json_task, "project", json_string(task->project));

    // Append the new task to the JSON array
    json_array_append_new(jarray, json_task);

    // Open the file for writing (overwrite)
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        json_decref(jarray);
        return;
    }

    // Write the updated JSON array back to the file
    if (json_dumpf(jarray, file, JSON_INDENT(4)) != 0) {
        perror("Error writing JSON to file");
    }

    fclose(file);
    json_decref(jarray);
}

int delete_task(const char *filename, int target_id) {
    json_error_t error;
    json_t *root;

    // 1. Load and parse JSON file
    root = json_load_file(filename, 0, &error);
    if (!root) {
        fprintf(stderr, "Error loading JSON: %s (line %d)\n", error.text, error.line);
        return 1;
    }

    // 2. Verify it's an array
    if (!json_is_array(root)) {
        fprintf(stderr, "Root is not an array\n");
        json_decref(root);
        return 1;
    }

    // 3. Find and remove target item
    size_t index;
    json_t *value;
    int found = 0;

    json_array_foreach(root, index, value) {
        json_t *id_obj = json_object_get(value, "id");
        if (id_obj && json_is_integer(id_obj)) {
            if (json_integer_value(id_obj) == target_id) {
                json_array_remove(root, index);
                found = 1;
                printf("Removed item with ID %d\n", target_id);
                break;
            }
        }
    }

    if (!found) {
        printf("No item with ID %d found\n", target_id);
    }

    // 4. Save modified JSON back to file
    if (json_dump_file(root, filename, JSON_INDENT(4)) != 0) {
        fprintf(stderr, "Error saving file\n");
        json_decref(root);
        return 1;
    }

    // 5. Cleanup
    json_decref(root);
    return 0;
}

void update_recurrent(const char* filename){
    for(size_t i=0; i<to_do_list.n_items; i++){
	if(to_do_list.items[i].recurrent && to_do_list.items[i].status == DONE && (second_until(to_do_list.items[i].due) < 0)){
	    struct tm time = *localtime(&to_do_list.items[i].due);
	    switch(to_do_list.items[i].recurrent){
		case DAILY:
		    time.tm_mday +=1;
		    to_do_list.items[i].due = mktime(&time);
		    while(second_until(to_do_list.items[i].due) < 0){
			time.tm_mday +=1;
			to_do_list.items[i].due = mktime(&time);
		    }
		    break;
		case WEEKLY: 
		    time.tm_mday +=7;
		    to_do_list.items[i].due = mktime(&time);
		    while(second_until(to_do_list.items[i].due) < 0){
			time.tm_mday +=7;
			to_do_list.items[i].due = mktime(&time);
		    }

		    break;		
		case MONTHLY:
		    time.tm_mon +=1;
		    to_do_list.items[i].due = mktime(&time);
		    while(second_until(to_do_list.items[i].due) < 0){
			time.tm_mon +=1;
			to_do_list.items[i].due = mktime(&time);
		    }

		    break;
		case YEARLY:
		    time.tm_year +=1;
		    to_do_list.items[i].due = mktime(&time);
		    while(second_until(to_do_list.items[i].due) < 0){
			time.tm_year +=1;
			to_do_list.items[i].due = mktime(&time);
		    }

		    break;
	     }
	    to_do_list.items[i].status = TODO;
	    delete_task(filename, i);
	    save(&to_do_list.items[i], filename);

	}
    }
}

