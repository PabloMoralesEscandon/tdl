#include "parser.h"
#include "memory.h"
#include "task.h"

int main (int argc, char **argv) {
    load(&to_do_list, FILE_NAME);
    char *command = argv[1];
    return dispatch_command(command, parse_options(argc, argv));
}