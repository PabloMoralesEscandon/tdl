TDL (ToDoList)
==============

A simple command-line to-do list manager made from scratch (using C standard libraries and jansson) as both a learning project and for personal usage.

## Installing

### Prerequisites
- Git
- GNU Make
- A C compiler (e.g., GCC or Clang)

### Build
```bash
git clone https://github.com/PabloMoralesEscandon/tdl.git
cd tdl
make release

### Symlink to bin
```bash
ln -sf "$PWD/tdl" ~/.local/bin/tdl
ln -sf "$PWD/tdl" ~/bin/tdl

Usage
-----

    tdl [command] [id or name] <options>

Commands
--------

    add            Add a new task
    show           Show information of a task
    mod            Modify an existing task
    start          Mark a task as started
    done           Mark a task as done
    del            Delete a task
    list           List all tasks
    list_projects  List all projects
    show_project   Show information of a project

Options
-------

    -p, --priority <val>       Task priority
    -r, --recurrent <val>      Recurrent rule / interval
    -d, --due <val>            Due date/time
    -s, --status <val>         Task status
    -c, --category <val>       Category
    -P, --project <val>        Project
    -n, --name <val>           Task name/title
    -D, --description <val>    Task description
    -h, --help                 Show this help and exit

Examples
--------

    tdl add Buy groceries -p low -d "01-05-2026" -c personal -P errands
    tdl list
    tdl add Do taxes -p high -d "05-06-2026" -P errands
    tdl show Buy groceries
    tdl start 3
    tdl done Buy groceries
    tdl mod 3 -p 1 -P errands
    tdl del 3
    tdl list_projects
    tdl show_project errands

