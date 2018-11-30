/**
 * @author Martin Lujan
 * Sistemas Operativos 2018
 *
 */

/*
|**********************************************************************;
* Proyecto           : Implemetancion de un Shell - TP2
*
* Nombre de programa   : Baash
|**********************************************************************;
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <memory.h>

#define buffer 256
#define buffer2 20
#define buffer3 50

int getPaths(char *paths[]);

int getCommands(char *argv1[], char *input1);

void searchExe(char *commando, char *paths[], char *exec);

/**
 * @brief Baash, se implementara una terminal del sistema, en la cual
 * se le ingresaran comandos asigados al path y que devolvera por pantalla lo que
 * se solicite
 * @TODO Creacion de hijo y ejecuion del proceso
 * @TODO Ocuparce del FIX de el proceso hijo.
 * @TODO Documentacion
 * @return 0
 */
int main() {
    char hostname[buffer2];
    char *user;
    gethostname(hostname, buffer2);
    user = getlogin();
    char input[buffer];
    char *argv[buffer2];
    char *paths[buffer2];
    int argc = 0;
    int NumPath = 0;
    NumPath = getPaths(paths);
    printf("%i\n", NumPath);
    char exec[buffer3];
    int pid;
    while (1) {

        printf("%s@%s %s$ ", user, hostname, getcwd(NULL, 256));
        /* Esperaremos el ingreso por teclado */
        fgets(input, buffer, stdin);

        if (!strcmp(input, "\n")) {
            printf("\n");
            continue;

        } else {
            argc = getCommands(argv, input);
            printf("%i\n", argc);
            if ((!strcmp(input, "exit\n")) || (!strcmp(input, "\n"))) {
                return 0;
            } else if (!strcmp(argv[0], "cd")) {
                chdir(argv[1]);
                continue;
            }
            searchExe(*argv, paths, exec);
            if (exec == NULL) {
                continue;
            } else if ((pid = fork(;)) < 0) {
                printf(" \"*** ERROR: forking child process failed\\n\" ");
                exit(EXIT_FAILURE);
            }else{

                if (pid < 0) {
                    exit(EXIT_FAILURE);
                } else if (pid == 0)

                    execv(exec, argv);
                    continue;


            }
        }

    }

}

/**
 * @brief Utilizada para la obtencion del path del sitema.
 * @param paths arreglo donde se alamacenaran los punteros a PATH
 * @return numero de PATH encontrados
 */
int getCommands(char *argv1[], char *input1) {
    int argc = 0;
    argv1[argc] = strtok(input1, " ");

    while (argv1[argc] != NULL) {
        //printf("%s\n", argv1[argc]);
        argc++;
        argv1[argc] = strtok(NULL, " \n");

    }

    return argc;

}

/**
 * @brief Utilizada para la extraccion de comandos y argumentos de la linea ingresada por el teclado
 * @param argv1 Se le envia un puntero a un vector el cual almacena los comandos ingresados.
 * @param input1 Se envia un tipo entero para contabilizar la cantidad de argumentos ingresados.
 * @return argc que es la cantidad de elemetos ingresados.
 */
int getPaths(char *paths[]) {

    int NumPath = 0;
    char *path = getenv("PATH");

    paths[NumPath] = strtok(path, ":");
    while (paths[NumPath] != NULL) {
        NumPath++;
        paths[NumPath] = strtok(NULL, ":");

    }
    return NumPath + 1;
}

/**
 * @brief Metodo utilizado para la busqueda del ejecutable en la variable PATH.
 * @param commando: Comando ejcutable.
 * @param paths: Lista de punteros a variables PATH.
 * @param exec: Se almacenara el puntero a la ruta donde se encuentra el ejecutable.
 */
void searchExe(char *commando, char *paths[], char *exec) {
    char searchDir[50];
    char *archivo;
    char *dir;
    char *NextArg;
    int result = 0;

/* Comenzamos buscando en un PATH absoluto */
    if (commando[0] == '/') {
        strcpy(searchDir, "/");
        dir = strtok(commando, "/");
        NextArg = strtok(NULL, "/");

        /* Se verifica que la ruta no sea del tipo Archivo + */
        if (NextArg == NULL) {
            archivo = dir;

            /* En caso de que asi sea, se procede a recuperar el resto de datos */
        } else {
            while (NextArg != NULL) {
                dir = NextArg;
                strcat(searchDir, dir);
                NextArg = strtok(NULL, "/");
                strcat(searchDir, "/");
            }
            archivo = dir;
        }

        /* De no cumplirse lo anterior verificamos que sea PATH relativa */
    } else if (commando[0] == '.' && commando[1] == '/') {
        //Guardamos el directorio en el que nos encontramos
        getcwd(searchDir, 50);
        strcat(searchDir, "/");
        archivo = strtok(commando, "/");
        archivo = strtok(NULL, "/");

        /* Como ultimo recurso se busca en todos los PATHS encontrados en el sistema */
    } else {
        int i = 0;
        while (paths[i] != NULL) {
            strcpy(searchDir, paths[i]);
            strcat(searchDir, "/");
            strcat(searchDir, commando);
            result = access(searchDir, F_OK);
            if (!result) {
                strcpy(exec, searchDir);
                return;
            }
            i++;
        }
        exec = NULL;
        printf("%i: Command not found\n", *commando);
        return;
    }

    /* Terminada la busqueda se procede a verificar que sea una ruta accesible al usuario y termina la secuencia */
    strcat(searchDir, archivo);
    result = access(searchDir, F_OK);
    if (!result) {
        strcpy(searchDir, exec);
        return;
    } else {
        exec = NULL;
        printf("Acceso restringido");
    }
    return;
}