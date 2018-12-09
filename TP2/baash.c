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
#include <fcntl.h>

#define buffer 256

void getPaths(char *paths[]);

int getCommands(char *argv1[], char *input1);

int searchExe(char *commando, char *paths[], char *exec);

void exePipe(char *argv1[], char *argv2[], char *paths[]);

void Out(char file[]);

/**
 * @brief Baash, se implementara una terminal del sistema, el cual interpretara intrucciones por la entrada
 * que se le asigne y ejecutara dichas instrucciones. El mismo puede ejecutar procesos en segundo plano mediante el
 * operador "&". Ademas contiene funcionalidades de redireccion y tuberias (Un solo operador por vez).
 * @TODO Implementacon de PIPEs
 * @TODO Implementacion de redireccion
 * @TODO Documentacion
 * @TODO Corregir la variable argc y usar una correcta para q el padre no espere
 * @return 0
 */
int main() {
    char hostname[buffer];
    char *user;
    gethostname(hostname, buffer);
    user = getlogin();
    char input[buffer];
    char *argv[buffer];
    char *argv2[buffer];
    char *paths[buffer];
    int argc = -1;
    getPaths(paths);
    char exec[buffer];
    int pipe, reDirectOut, reDirectIn;
    int exe;
    int background;
    int pid;

    while (1) {

        /* Se limpian las banderas */
        pipe = 0;
        reDirectOut = 0;
        reDirectIn = 0;
        strcpy(input, "\0");
        printf("%s@%s %s$ ", user, hostname, getcwd(NULL, 256));
        fflush(stdin);
        fflush(stdout);

        /* Esperaremos el ingreso por teclado */
        fgets(input, buffer, stdin);

        if (!strcmp(input, "\n")) {
            printf("\n");
            continue;

        } else {
            /* Compruebo la presencia de pipe o redireccion de entrada */
            char *pipePos = strchr(input, '|');
            if (pipePos != NULL) {
                *pipePos = '\0';
                pipe = 1;
                background = getCommands(argv, input);
                background = getCommands(argv2, pipePos + 1);
            } else {
                char *Out = strchr(input, '>');
                char *In = strchr(input, '<');
                if (Out != NULL) {
                    *Out = '\0';
                    reDirectOut = 1;
                    background = getCommands(argv, input);
                    background = getCommands(argv2, Out + 1);
                } else if (In != NULL) {
                    *In = '\0';
                    reDirectIn = 1;
                    background = getCommands(argv, input);
                    background = getCommands(argv2, In + 1);
                } else {
                    background = getCommands(argv, input);
                }
            }

            /* Realizo comprobaciones antes de crear el hijo */
            if ((!strcmp(input, "exit"))) {
                return 0;
            } else if (!strcmp(argv[0], "cd")) {
                chdir(argv[1]);
                continue;
            } else {

                /* Busqueda del ejecutable */
                exe = searchExe(*argv, paths, exec);

                /* Se corrobora que se haya encontrado el ejecutable */
                if (exe) {
                    continue;
                } else {
                    pid = fork();
                    if (pid < 0) {
                        printf(" \"*** ERROR: forking child process failed\\n\" ");
                        exit(EXIT_FAILURE);

                    } else if (pid == 0) {

                        /* Proceso hijo */
                        fflush(stdout);
                        if (pipe == 1) {

                            /* Ejecucion del pipe */
                            exePipe(argv, argv2, paths);
                        } else if (reDirectOut == 1) {

                            /* Redireccion de salida */
                            Out(argv2[0]);
                            execv(exec, argv);
                            perror(exec);
                            exit(EXIT_FAILURE);
                        } else if (reDirectIn == 1) {

                            /* Redireccion de entrada */
                            freopen(argv2[0], "r", stdin);
                            execv(exec, argv);
                            perror(exec);
                            exit(EXIT_FAILURE);
                        } else {

                            /* Si no se cumplen las anteriores se ejecutara */
                            execv(exec, argv);
                            perror(exec);
                            exit(EXIT_FAILURE);
                        }
                    }

                }

                /* Verificamos si hay que esperar al hijo */
                if (!background) {
                    wait(&pid);
                } else {
                    background = 0;
                    waitpid(pid, &argc, WNOHANG);

                }
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
}

/**
 * @brief Metodo utilizado para la busqueda del ejecutable en la variable PATH.
 * @param commando: Comando ejcutable.
 * @param paths: Lista de punteros a variables PATH.
 * @param exec: Se almacenara el puntero a la ruta donde se encuentra el ejecutable.
 * @return entero, 0 si se encontro el ejecutable, 1 caso contrario.
 */
int searchExe(char *commando, char *paths[], char *exec) {
    char searchDir[buffer];
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
                return 0;
            }
            i++;
        }
        printf("%i: Command not found (programa)\n", *commando);
        return 1;
    }

    /* Terminada la busqueda se procede a verificar que sea una ruta accesible al usuario y termina la secuencia */
    strcat(searchDir, archivo);
    result = access(searchDir, F_OK);
    if (!result) {
        strcpy(exec, searchDir);
        return 0;
    } else {
        printf("Acceso restringido");
        return 1;
    }
}

/**
 * Funcion encargada de ejecutar el pipe, creando un proceso nieto.
 * @param argv arreglo de argumentos para ejecutar el proceso.
 * @param paths arreglo donde se encuentran los Paths de ejecutables del sistema.
 * @param argv2 arreglo donde se encuentran los argumentos para el pipe.
 */
void exePipe(char *argv1[], char *argv2[], char *paths[]) {

    char exec[buffer];
    int fd[2], pid;

    if (pipe(fd) < 0) {
        perror("Error pipe");
        exit(EXIT_FAILURE);
    } else if ((pid = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {

        /* Codigo del hijo */
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO); //Redireccion de la salida al pipe.
        close(fd[1]);
        searchExe(argv1[0], paths, exec);
        execv(exec, argv1);
        perror(exec);
        exit(EXIT_FAILURE);
    } else {

        /* Codigo del padre */
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        searchExe(argv2[0], paths, exec);
        execv(exec, argv2);
        perror(exec);
        exit(EXIT_FAILURE);
    }

}

/**
 * Funcion encargada de modificar la salida estandar.
 * @param file archivo al cual se envia la salida.
 */
void Out(char file[]) {
    int fid;
    int flags, perm;
    flags = O_WRONLY | O_CREAT | O_TRUNC;
    perm = S_IWUSR | S_IRUSR;

    fid = open(file, flags, perm);
    if (fid < 0) {
        perror("open");
        exit(1);
    }
    close(STDOUT_FILENO);
    if (dup(fid) < 0) {
        perror("dup");
        exit(1);
    }
    close(fid);
}