/**
 * @author Martin Lujan
 * Sistemas Operativos 2018
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <memory.h>

/**
 * @brief Utilizada para la obtencion del path del sitema.
 * @param paths arreglo donde se alamacenaran los punteros a PATH
 * @return numero de PATH encontrados
 */
int getPaths(char* paths[]);

/**
 * @brief Utilizada para la extraccion de comandos y argumentos de la linea ingresada por el teclado
 * @param argv1 Se le envia un puntero a un vector el cual almacena los comandos ingresados.
 * @param input1 Se envia un tipo entero para contabilizar la cantidad de argumentos ingresados.
 * @return argc que es la cantidad de elemetos ingresados.
 */
int getCommands(char *argv1[], char *input1);

/**
 * @brief Metodo utilizado para la busqueda del ejecutable en la variable PATH.
 * @param commando: Comando ejcutable.
 * @param paths: Lista de punteros a variables PATH.
 * @param exec: Se almacenara el puntero a la ruta donde se encuentra el ejecutable.
 */
void searchExe(char* commando, char* paths[], char* exec);
/**
 * @brief Baash, se implementara una terminal del sistema, en la cual
 * se le ingresaran comandos asigados al path y que devolvera por pantalla lo que
 * se solicite
 * @TODO BUSQUEDA EN EL PATH
 * @TODO Documentacion
 * @return 0
 */
#define buffer 256
#define buffer2 20

int main() {
    char hostname[20];
    char *user;
    gethostname(hostname, buffer2);
    user = getlogin();
    char input[buffer];
    char *argv[buffer2];
    char *paths[buffer2];
    int argc = 0;
    int NumPath = 0;
    NumPath = getPaths(paths);
    printf("%i\n",NumPath);
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
            else if(*argv[argc] == '&'){

            }
            else if()
        }
    }

}

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

int getPaths(char* paths[]) {

    int NumPath = 0;
    char *path = getenv("PATH");

    paths[NumPath] = strtok(path, ":");
    while (paths[NumPath] != NULL) {
        NumPath++;
        paths[NumPath] = strtok(NULL, ":");

    }
    return NumPath + 1;
}