/**
 * @author Martin Lujan
 * Sistemas Operativos 2018
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#define BUFFERSIZE 256;
/**
 * @brief Utilizada para la obtencion del path del sitema.
 * @return
 */
int getPath();

/**
 * @brief Baash, se implementara una terminal del sistema, en la cual
 * se le ingresaran comandos asigados al path y que devolvera por pantalla lo que
 * se solicite
 * @TODO Implementar la entrada por teclado de parametros.
 * @param argc tipo entero el cual lleva la cuenta de los parametros que se ingresan.
 * @param argv puntero tipo char, de tamaño dinamico, en el que se guardaran los parametros ingresados.
 * @return 0
 */
int main ()
{
  char hostname[20];
  char* user;
  gethostname (hostname,20);
  user = getlogin ();
  char input[256];



  while(1){

      printf ("%s@%s %s$ ", user, hostname, getcwd(NULL,256));
      fgets (input, 256, stdin);
      printf ("se ingreso %s", input);
      break;
  }

  return 0;
}

