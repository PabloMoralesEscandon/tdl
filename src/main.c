#include "parser.h"
#include "memory.h"
#include "task.h"

int main (int argc, char **argv) {
    load(&to_do_list, FILE_NAME);
    char *command = argv[1];
    char *options[NUMBER_OPT] = {0};
    char **words = NULL;
    int id = parse_options(argc, argv, options);
    return dispatch_command(command, options);
}