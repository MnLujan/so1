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
int getPath ();
/**
 * @brief Utilizada para la extraccion de comandos y argumentos de la linea ingresada por el teclado
 * @param Se le envia un puntero a un vector el cual almacena los comandos ingresados.
 * @param Se envia un tipo entero para contabilizar la cantidad de argumentos ingresados.
 * @return 0.
 */
char getCommands(char);

/**
 * @brief Baash, se implementara una terminal del sistema, en la cual
 * se le ingresaran comandos asigados al path y que devolvera por pantalla lo que
 * se solicite
 * @TODO Implementar la entrada por teclado de parametros.
 * @return 0
 */
int main ()
{
  char hostname[20];
  char *user;
  gethostname (hostname, 20);
  user = getlogin ();
  char input[256];
  char argv;

  while (1)
    {

      printf ("%s@%s %s$ ", user, hostname, getcwd (NULL, 256));
      /* Esperaremos el ingreso por teclado */
      fgets (input, 256, stdin);

      if (!strcmp (input, "\n"))
        {
          printf ("\n");
          continue;
        }
      else if (strcmp (input, "exit"))
        {
          break;
        }
      else
        {
          argv = getCommands (input);

          continue;
        }

    }

  return 0;
}

#define buffer 256

char getCommands(char line){

  char **tokens = malloc(256);
  char *token;
  int pos = 0;

  if(!tokens){
    printf ("Allocation error \n");
    exit (EXIT_FAILURE);
  }

  token = strtok (line, buffer);

  while(token != NULL){
    tokens[pos] = token;
    pos++;

    token = strtok (NULL, buffer);
  }
  tokens[pos] = NULL;
  return tokens;

}