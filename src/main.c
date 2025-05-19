#include <string.h>

#include "parser.h"
#include "memory.h"
#include "task.h"

int main (int argc, char **argv) {
    load(&to_do_list, FILE_NAME);
    char *command = argv[1];
    char *options[NUMBER_OPT] = {0};
    char *words = parse_words(argc, argv);
    int id = parse_id_name(words);
    if(id==-1) options[NAME] = strdup(words);
    parse_options(argc, argv, options);
    return dispatch_command(command, options, id);
}