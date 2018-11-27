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
 * @return
 */
int getPath();

/**
 * @brief Utilizada para la extraccion de comandos y argumentos de la linea ingresada por el teclado
 * @param Se le envia un puntero a un vector el cual almacena los comandos ingresados.
 * @param Se envia un tipo entero para contabilizar la cantidad de argumentos ingresados.
 * @return 0.
 */
void getCommands(char* argv1[], char* input1);

/**
 * @brief Baash, se implementara una terminal del sistema, en la cual
 * se le ingresaran comandos asigados al path y que devolvera por pantalla lo que
 * se solicite
 * @TODO Obetencion de PATH, busqueda relativa y absoluta.
 * @return 0
 */

#define buffer 256
int main() {
    char hostname[20];
    char *user;
    gethostname(hostname, 20);
    user = getlogin();
    char input[buffer];
    char* argv[20];

    while (1) {

        printf("%s@%s %s$ ", user, hostname, getcwd(NULL, 256));
        /* Esperaremos el ingreso por teclado */
        fgets(input, buffer, stdin);

        if (!strcmp(input,"\n")) {
            printf("\n");
            continue;

        } else {
            getCommands(argv,input);

            if (!strcmp(input,"exit\n")) {
                return 0;
            } else if (!strcmp(argv, "cd")){
                //chdir(argv[1]);
                continue;
            }
        }
    }

}

void getCommands(char* argv1[], char* input1) {
int argc = 0;
argv1[argc] = strtok(input1," ");
//argc++;

while (argv1[argc] != NULL){
    printf("%s\n", argv1[argc]);
    argc ++;
    argv1[argc] = strtok (NULL," \n");

}

return;

}