/**
 * @author Martin Lujan
 * @date 25/09/2018
 */

#include <stdio.h>
#include <unistd.h>
#include <libnet.h>
#include <zconf.h>
#include "LAB1.h"

/*
|**********************************************************************;
* Proyecto           : Analizador de /proc
*
* Nombre de programa   : TP1
|**********************************************************************;
*/

/**
 * @brief Main recibe 2 parametros opcionalmente, o ninguno. En base a estos parametros el el programa
 * imprimira por pantalla distintos datos de la pc, segun corresponda.
 * @param argc indicara la cantidad de parametros ingresados. Este parametro es proporcionado por el OS al momento
 * de ejecutar el programa.
 * @param argv puntero a un vector el cual es de tamaño dinamico, aqui se almacenaran los parametros que se
 * le pasen al programa. Este parametro es proporcionado por el OS al momento
 * de ejecutar el programa.
 * @return 0
 */
int main (int argc, char *argv[])
{
  int flagS = 0;
  int flagL = 0;
  int aux = 0;
  int c;

/* =========================================
 *
 * ANALISIS DE PARAMETROS INGRESADOS
 *
 * =========================================
 */
  while ((c = getopt (argc, argv, "sl:")) != -1)
    {
      switch (c)
        {
          case 's':
            flagS = 1;
          break;
          case 'l':
            flagL = 1;
          break;
          default:
            aux = 1;

        }
    }

/*
 * Ejecucion dependiendo de los parametros ingresados.
 */
  if (aux == 0 && flagL == 0 && flagS == 0)
    {
      printincio ();
    }
  else if (flagS == 1 && flagL == 0 && aux == 0)
    {
      printincio ();
      get_process ();
      get_time ();
      context ();
      Process ();

    }
  else if (flagL == 1 && aux == 0 && flagS == 0)
    {
      char *endptr;
      long int a = strtol (argv[2], &endptr, 10);
      if ((errno == ERANGE && (a == LONG_MAX || a == LONG_MIN))
          || (errno != 0 && a == 0))
        {
          printf ("Los numeros son muy grandes");
          return 0;
        }
      else if (endptr == argv[2])
        {
          printf ("No se encuentran numeros.");
          return 0;
        }
      long int b = strtol (argv[3], &endptr, 10);
      if ((errno == ERANGE && (b == LONG_MAX || b == LONG_MIN))
          || (errno != 0 && b == 0))
        {
          printf ("Los numeros son muy grandes");
          return 0;
        }
      else if (endptr == argv[3])
        {
          printf ("No se encuentran numeros.");
          return 0;
        }
      printincio ();
      get_process ();
      get_time ();
      context ();
      Process ();

      for (long int i = 0; i < b; i = i + a)
        {
          printf ("-Peticiones de Diso: ");
          get_diskstas ();
          printf ("-Memoria: \n");
          get_memory ();
          LoadAvg ();
          printf ("Pausa de %li segundos \n \n", a);
          sleep ((unsigned int) a);
        }
    }
  else
    {
      printf ("Parametros no validos \nLas opciones correctas son: \n'ksamp' \n'ksamp -s'\n'ksamp -l a b (a,b intervalos de ejecucion)\n'");
    }
  printf ("[Sistemas Operativos 1 - 2018] \n");
  return 0;
}
