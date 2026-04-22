#ifndef TASK_H
#define TASK_H

#include <stdlib.h>
#include <time.h>

/**
 * @file task.h
 * @brief Tipos y utilidades de dominio para tareas y proyectos.
 */

/** @brief Longitud maxima, incluyendo el terminador nulo, para categoria/proyecto. */
#define NAME_CHARS 21
/** @brief Longitud maxima, incluyendo el terminador nulo, para el nombre de una tarea. */
#define LONG_NAME_CHARS 41
/** @brief Longitud maxima, incluyendo el terminador nulo, para la descripcion. */
#define DESC_CHARS 201

/** @brief Secuencia ANSI para restaurar el estilo del terminal. */
#define ANSI_RESET "\x1b[0m"
/** @brief Secuencia ANSI para activar negrita. */
#define ANSI_BOLD "\x1b[1m"
/** @brief Secuencia ANSI para imprimir en azul. */
#define ANSI_BLUE "\x1b[34m"
/** @brief Secuencia ANSI para negrita en azul. */
#define ANSI_BOLD_BLUE "\x1b[1;34m"

/**
 * @enum Status
 * @brief Estados posibles de una tarea.
 */
enum Status {
    TODO,        /**< Tarea pendiente. */
    IN_PROGRESS, /**< Tarea en curso. */
    DONE         /**< Tarea completada. */
};

/**
 * @enum Priority
 * @brief Niveles de prioridad soportados.
 */
enum Priority {
    LOW,    /**< Prioridad baja. */
    MEDIUM, /**< Prioridad media. */
    HIGH,   /**< Prioridad alta. */
    URGENT  /**< Prioridad urgente. */
};

/**
 * @enum Recurrent
 * @brief Periodicidad de repeticion de una tarea.
 */
enum Recurrent {
    NO,      /**< Tarea no recurrente. */
    DAILY,   /**< Repite cada dia. */
    WEEKLY,  /**< Repite cada semana. */
    MONTHLY, /**< Repite cada mes. */
    YEARLY   /**< Repite cada ano. */
};

/**
 * @enum Due
 * @brief Clasificacion relativa del vencimiento de una tarea.
 */
enum Due {
    LATER, /**< Vence mas adelante o no tiene fecha. */
    YEAR,  /**< Vence dentro del proximo ano. */
    MONTH, /**< Vence dentro del proximo mes. */
    WEEK,  /**< Vence dentro de la proxima semana. */
    DAY    /**< Vence dentro del proximo dia. */
};

/**
 * @brief Anade un elemento a un array dinamico basado en campos `items`, `n_items` y `size`.
 *
 * Si el buffer no tiene capacidad suficiente, se realoja duplicando su tamano.
 *
 * @param da Estructura de array dinamico.
 * @param element Elemento a insertar al final.
 */
#define append(da, element)                                                    \
    do {                                                                       \
        if (da.n_items >= da.size) {                                           \
            if (da.size == 0)                                                  \
                da.size = 256;                                                 \
            else                                                               \
                da.size *= 2;                                                  \
            da.items = realloc(da.items, da.size * sizeof(*da.items));         \
        }                                                                      \
        da.items[da.n_items++] = element;                                      \
    } while (0)

/**
 * @struct Task
 * @brief Representa una tarea persistida y mostrada por la aplicacion.
 */
typedef struct {
    int id;             /**< Identificador unico de la tarea. */
    char *name;         /**< Nombre corto de la tarea. */
    char *description;  /**< Descripcion libre. */
    int priority;       /**< Prioridad, usando @ref Priority. */
    time_t due;         /**< Fecha de vencimiento; 0 indica "sin fecha". */
    int recurrent;      /**< Regla de recurrencia, usando @ref Recurrent. */
    int status;         /**< Estado actual, usando @ref Status. */
    char *category;     /**< Categoria funcional o "none". */
    char *project;      /**< Proyecto asociado o "none". */
} Task;

/**
 * @struct ToDoList
 * @brief Array dinamico con todas las tareas cargadas en memoria.
 */
typedef struct {
    Task *items;     /**< Buffer con las tareas. */
    size_t n_items;  /**< Numero de tareas validas. */
    size_t size;     /**< Capacidad reservada en `items`. */
} ToDoList;

/**
 * @struct ToDoProjects
 * @brief Array dinamico con los nombres de proyectos activos.
 */
typedef struct {
    char **items;    /**< Buffer con nombres de proyecto. */
    size_t n_items;  /**< Numero de proyectos almacenados. */
    size_t size;     /**< Capacidad reservada en `items`. */
} ToDoProjects;

/** @brief Lista global de tareas cargadas desde almacenamiento. */
extern ToDoList to_do_list;

/** @brief Lista global de proyectos activos derivada de las tareas. */
extern ToDoProjects to_do_proj;

/**
 * @brief Imprime una tarea en formato detallado.
 * @param task Tarea a mostrar.
 */
void print_task(Task *task);

/**
 * @brief Convierte una prioridad interna a su etiqueta legible.
 * @param priority Valor de prioridad.
 * @return Cadena constante con la representacion textual.
 */
char *get_priority(int priority);

/**
 * @brief Convierte una prioridad textual a su valor interno.
 * @param priority Cadena introducida por el usuario.
 * @return Valor de @ref Priority o `-1` si no es valida.
 */
int get_priority_int(char *priority);

/**
 * @brief Convierte una recurrencia interna a su etiqueta legible.
 * @param recurrence Valor de recurrencia.
 * @return Cadena constante con la representacion textual.
 */
char *get_recurrence(int recurrence);

/**
 * @brief Convierte una recurrencia textual a su valor interno.
 * @param recurrence Cadena introducida por el usuario.
 * @return Valor de @ref Recurrent o `-1` si no es valida.
 */
int get_recurrence_int(char *recurrence);

/**
 * @brief Convierte un estado interno a su etiqueta legible.
 * @param status Valor de estado.
 * @return Cadena constante con la representacion textual.
 */
char *get_status(int status);

/**
 * @brief Convierte un estado textual a su valor interno.
 * @param status Cadena introducida por el usuario.
 * @return Valor de @ref Status o `-1` si no es valido.
 */
int get_status_int(char *status);

/**
 * @brief Comprueba si una fecha descompuesta es valida.
 * @param date Estructura `tm` con dia, mes y ano inicializados.
 * @return `1` si la fecha es valida; `0` en caso contrario.
 */
int is_valid_date(struct tm date);

/**
 * @brief Calcula los segundos que faltan hasta un instante objetivo.
 * @param target Marca temporal de destino.
 * @return Diferencia `target - now` en segundos.
 */
double second_until(time_t target);

/**
 * @brief Clasifica una fecha segun su cercania relativa.
 * @param target Fecha objetivo.
 * @return Valor de @ref Due correspondiente.
 */
int when_due(time_t target);

/**
 * @brief Indica si un proyecto ya esta registrado en la lista global.
 * @param name Nombre del proyecto.
 * @return `1` si existe; `0` en caso contrario.
 */
int is_in_proj_list(char *name);

/**
 * @brief Imprime el resumen y progreso de un proyecto.
 * @param id Indice del proyecto en @ref to_do_proj.
 */
void print_proj(int id);

/** @brief Imprime la cabecera de la tabla de tareas. */
void print_task_table_header();

/**
 * @brief Imprime una fila de la tabla de tareas.
 * @param t Tarea a imprimir.
 */
void print_task_table_row(Task *t);

/** @brief Imprime la cabecera de la tabla de proyectos. */
void print_proj_table_header();

/**
 * @brief Imprime una fila de la tabla de proyectos.
 * @param proj Nombre del proyecto.
 * @param id Indice mostrado para el proyecto.
 */
void print_proj_table_row(char *proj, int id);

#endif
