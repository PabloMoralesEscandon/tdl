#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"

// Assuming ToDoList and Task are defined something like this:
// typedef struct {
//     int id;
//     char *name;
//     char *description;
//     int priority;
//     int due;
//     char *category;
//     int recurrent;
//     int status;
//     char *project;
// } Task;
//
// typedef struct {
//     Task *items;
//     size_t n_items;
//     size_t capacity;
// } ToDoList;

void load(ToDoList *todo_list, const char *filename) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");
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
    free(todo_list->items);

    // Allocate new array for tasks
    todo_list->items = malloc(items_count * sizeof(Task));
    if (todo_list->items == NULL) {
        perror("Memory allocation failed");
        json_decref(jarray);
        return;
    }

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
        json_t *json_due = json_object_get(json_item, "due");
        json_t *json_cat = json_object_get(json_item, "category");
        json_t *json_rec = json_object_get(json_item, "recurrent");
        json_t *json_stat = json_object_get(json_item, "status");
        json_t *json_pro = json_object_get(json_item, "project");

        if (
            json_is_integer(json_id) &&
            json_is_string(json_name) &&
            json_is_string(json_desc) &&
            json_is_integer(json_prio) &&
            json_is_integer(json_due) &&
            json_is_string(json_cat) &&
            json_is_integer(json_rec) &&
            json_is_integer(json_stat) &&
            json_is_string(json_pro)
        ) {
            new_task.id = (int)json_integer_value(json_id);
            new_task.name = strdup(json_string_value(json_name));
            new_task.description = strdup(json_string_value(json_desc));
            new_task.priority = (int)json_integer_value(json_prio);
            new_task.due = (int)json_integer_value(json_due);
            new_task.category = strdup(json_string_value(json_cat));
            new_task.recurrent = (int)json_integer_value(json_rec);
            new_task.status = (int)json_integer_value(json_stat);
            new_task.project = strdup(json_string_value(json_pro));
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
        }
        append(to_do_list, new_task);
    }

    json_decref(jarray);
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
    json_object_set_new(json_task, "due", json_integer(task->due));
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
