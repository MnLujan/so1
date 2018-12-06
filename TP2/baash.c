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
#include <wait.h>

#define buffer 256
#define buffer2 20
#define buffer3 50

void getPaths(char *paths[]);

int getCommands(char *argv1[], char *input1);

void searchExe(char *commando, char *paths[], char *exec);

int checkPipe(char *commando[], char *argv2);

void exePipe(char *argv[], char *paths[]);

/**
 * @brief Baash, se implementara una terminal del sistema, en la cual
 * se le ingresaran comandos asigados al path y que devolvera por pantalla lo que
 * se solicite
 * @TODO Implementacon de PIPEs
 * @TODO Implementacion de redireccion
 * @TODO Documentacion
 * @TODO Corregir la variable argc y usar una correcta para q el padre no espere
 * @return 0
 */
int main() {
    char hostname[buffer2];
    char *user;
    gethostname(hostname, buffer2);
    user = getlogin();
    char input[buffer];
    char *argv[buffer2];
    char *argv2[buffer2];
    char *paths[buffer2];
    int argc = -1;
    getPaths(paths);
    char exec[buffer3];
    int pid;
    int background;
    char filename[buffer3];
    int pipe, reDirect;

    while (1) {
        strcpy(input, "\n");
        printf("%s@%s %s$ ", user, hostname, getcwd(NULL, 256));

        /* Esperaremos el ingreso por teclado */
        fgets(input, buffer, stdin);

        if (!strcmp(input, "\n")) {
            printf("\n");
            continue;

        } else {

            /* Parceo los comandos ingresados y devuelvo si hay background o no */
            background = getCommands(argv, input);

            /* Realizo comprobaciones antes de crear el hijo */
            if ((!strcmp(input, "exit"))) {
                return 0;
            } else if (!strcmp(argv[0], "cd")) {
                chdir(argv[1]);
                continue;
            } else {

                /* Busqueda del ejecutable */
                searchExe(*argv, paths, exec);

                /* Verificacion de pipe */
                pipe = checkPipe(argv, argv2);

                /* Se corrobora que se haya encontrado el ejecutable */
                if (exec[0] == 0) {
                    continue;
                } else if ((pid = fork()) < 0) {
                    printf(" \"*** ERROR: forking child process failed\\n\" ");
                    exit(EXIT_FAILURE);

                    /* Proceso hijo */
                } else {
                    if(pipe == 1){
                        exePipe(argv2,paths);
                     //funcion de ejecucion pipe
                    }else {
                        execv(exec, argv);
                        perror(exec);
                    }
                }

            }

            /* Verificamos si hay que esperar al hijo */
            if (!background) {
                wait(&pid);
            } else {
                waitpid(pid, &argc, WNOHANG);
                background = 0;
            }
        }

    }

}


/**
 * @brief Utilizada para la extraccion de comandos y argumentos de la linea ingresada por el teclado y verifica si
 * se encuentra la presencia del argumento "&".
 * @param argv1 Se le envia un puntero a un vector el cual almacena los comandos ingresados.
 * @param input1 Se envia un tipo entero para contabilizar la cantidad de argumentos ingresados.
 * @return back variable que me indica si hay que ejecutar un proceso en segundo plano.
 */
int getCommands(char *argv1[], char *input1) {
    int argc = 0;
    int back = 0;
    argv1[argc] = strtok(input1, " " "\n");

    while (argv1[argc] != NULL) {
        argc++;
        argv1[argc] = strtok(NULL, " \n");

    }
    if (!strcmp(argv1[argc - 1], "&")) {
        back = 1;
        argv1[argc - 1] = NULL;
        return back;
    } else {

        back = 0;
        return back;
    }


}

/**
 * @brief Utilizada para la obtencion del path del sitema.
 * @param paths arreglo donde se alamacenaran los punteros a PATH
 */
void getPaths(char *paths[]) {

    int NumPath = 0;
    char *path = getenv("PATH");
/* Cada path encontrado se guarda en un arreglo de punteros  */
    paths[NumPath] = strtok(path, ":");
    while (paths[NumPath] != NULL) {
        NumPath++;
        paths[NumPath] = strtok(NULL, ":");

    }
    return;
}

/**
 * @brief Metodo utilizado para la busqueda del ejecutable en la variable PATH.
 * @param commando: Comando ejcutable.
 * @param paths: Lista de punteros a variables PATH.
 * @param exec: Se almacenara el puntero a la ruta donde se encuentra el ejecutable.
 */
void searchExe(char *commando, char *paths[], char *exec) {
    char searchDir[buffer3];
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
        exec[0] = 0;
        printf("%i: Command not found (programa)\n", *commando);
        return;
    }

    /* Terminada la busqueda se procede a verificar que sea una ruta accesible al usuario y termina la secuencia */
    strcat(searchDir, archivo);
    result = access(searchDir, F_OK);
    if (!result) {
        strcpy(exec, searchDir);
        return;
    } else {
        exec[0] = 0;
        printf("Acceso restringido");
    }
    return;
}

/**
 * Funcion utilizada para la compropacion del uso de pipes.
 * @param commando se le pasan los comandos ya parceados para la busqueda.
 * @param argv2 Arreglo en el cual se guardan los argumentos para el pipe.
 * @return devuelvo un 1 en caso de que se deba implementar pipe. 0 Caso contrario.
 */
int checkPipe(char *commando[], char *argv2) {
    int pipe = 0;
    int argc = 0;
    char *Arg;
    char *NextArg;
    while (commando[argc] != NULL) {

        if (!strcmp(commando[argc],"|")) {
            argc ++;
            Arg = strtok(commando[argc], " " "\n");
            argc++;
            NextArg = strtok(commando[argc], " " "\n");
            strcpy(argv2, Arg);

            while (NextArg != NULL){
                strcat(argv2,NextArg);
                argc++;
                NextArg = strtok(commando[argc], "" "\n");
            }
            pipe = 1;
            return pipe;

        }else{
            argc ++;
        }
    }
    return pipe;
}

/**
 * Funcion encargada de ejecutar el pipe, creando un proceso nieto.
 * @param argv arreglo de argumentos para el pipe.
 * @param paths arreglo donde se encuentran los Paths de ejecutables del sistema.
 */
void exePipe(char *argv[], char *paths[]){

}