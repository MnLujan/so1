/**
 * @author Martin Lujan
 * @date 25/09/2018
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef CPU
#define CPU
#define BUFFERSIZE 2048
char buffer[BUFFERSIZE];

/**
 * @brief Imprime el tipo y modelo del CPU, extraido desde el directorio /proc/cpuinfo
 * @param model arreglo en el cual se guarda el string del modelo.
 * @param tipo arreglo en el cual se guarda el string del tipo de cpu.
 */
void get_info_CPU ()
{

  FILE *fp;
  size_t bytes_read;
  char *match;
  char model[50];
  char tipo[50];

/* Lee el contenido dentro de /proc/cpuinfo y lo guarda en el buffer.*/
  fp = fopen ("/proc/cpuinfo", "r");
  bytes_read = fread (buffer, 1, BUFFERSIZE - 1, fp);
  fclose (fp);

/* Falla si la lectura fue erronea o si el buffer es pequeño */
  if (bytes_read == 0 || bytes_read == BUFFERSIZE)
    {
      printf ("Fallo1");
      return;
    }

/* Finaliza el texto */
  buffer[bytes_read] = '\0';

/* Ubica la linea que comienza con “Model name”. */
  match = strstr (buffer, "model name");

  if (match == NULL)
    {
      printf ("Fallo2");
      return;
    }
/* Analiza la linea y extrae el modelo */
  sscanf (match, "model name : %49[^\n]", model);


/* Modifico el puntero a la linea "vendor_id" */
  match = strstr (buffer, "vendor_id");

  if (match == NULL)
    {
      printf ("Fallo3");
      return;
    }
  /*Extraccion del tipo de CPU*/
  sscanf (match, "vendor_id : %49[^\n]", tipo);

  printf ("%s \n%s \n", model, tipo);
  return;
}

/**
 * @brief Impresion version kernel. La siguiente funcion extrae del directorio /proc/version
 * la version de kernel del sistema operativo.
 * @param version arreglo en el que se guarda el string extraido desde el directorio.
 */
void get_v_kernel ()
{
  FILE *fp;
  size_t bytes_read;
  char version[50];

  /* Lee el contenido dentro de /proc/version y lo guarda en el buffer.*/
  fp = fopen ("/proc/version", "r");
  bytes_read = fread (buffer, 1, BUFFERSIZE - 1, fp);
  fclose (fp);

/* Falla si la lectura fue erronea o si el buffer es pequeño */
  if (bytes_read == 0 || bytes_read == BUFFERSIZE - 1)
    {
      printf ("fallo lectura");
      return;
    }

/* Finaliza el texto */
  buffer[bytes_read] = '\0';

/* Busco la palabra version y devuelvo el puntero */
  sscanf (buffer, "Linux version %49s", version);
  printf ("%s \n", version);
  return;

}

/**
 * @brief Uptime extrae desde el directorio /proc/uptime el tiempo en segundos desde que el sistema fue iniciado.
 * @param uptime variable tipo double en el que se almacenan los segundos.
 */
void get_uptime ()
{
  FILE *fp;
  double uptime;

/* lectura desde /proc/uptime. */
  fp = fopen ("/proc/uptime", "r");
  fscanf (fp, "%lf", &uptime);
  fclose (fp);

  /* constantes de conversion */
  const int minute = 60;
  const int hour = minute * 60;
  const int day = hour * 24;

/* Output. */
  printf ("%s: %i days, %i:%02i:%02i\n", "Uptime", ((int) (uptime) / day), (((int) (uptime) % day) / hour),
          (((int) (uptime) % hour) / minute), ((int) (uptime) % minute));
  return;

}

/**
 * @brief Impresion de procesos, este metodo extrae del directorio /proc/stat los distintos itmepos en que
 * hubo procesos de sistema, usuarios, o directamente no se uso.
 * @param UserTime arreglo en el cual se almacena el string extraido.
 * @param Systime igual que el anterior.
 * @param NoUse igual que el anterior.
 */
void get_process ()
{

  FILE *fp;
  size_t bytes_read;
  char *match;
  char UserTime[50];
  char SysTime[50];
  char NoUse[50];

/* Lee el contenido dentro de /proc/stat y lo guarda en el buffer.*/
  fp = fopen ("/proc/stat", "r");
  bytes_read = fread (buffer, 1, BUFFERSIZE - 1, fp);
  fclose (fp);

/* Falla si la lectura fue erronea o si el buffer es pequeño */
  if (bytes_read == 0 || bytes_read == BUFFERSIZE - 1)
    {
      printf ("buffer pequeño");
      return;
    }

/* Finaliza el texto */
  buffer[bytes_read] = '\0';

/* Ubica la linea que comienza con “cpu”. */
  match = strstr (buffer, "cpu");

  if (match == NULL)
    {
      printf ("fallo lectura");
      return;
    }

/* Analiza la linea y extrae el modelo */
//%*s se utiliza para ignorar ese string
  sscanf (match, "cpu %49s %*s %49s %49s", UserTime, SysTime, NoUse);
  printf ("UserTime: %s \nSysTime: %s \nNoUse: %s \n", UserTime, SysTime, NoUse);
  return;
}

/**
 * @brief Impresion de memoria, el metodo extrae desde el directorio /proc/meminfo
 * la memoria total y libre del sistema en el momento de la ejecucion.
 * @param MemTotal alamacena el tiempo extraido.
 * @param MemFree igual que el anterior.
 */
void get_memory ()
{
  FILE *fp;
  size_t bytes_read;
  char *match;
  int MemTotal;
  int MemFree;


/* Lee el contenido dentro de /proc/meminfo y lo guarda en el buffer.*/
  fp = fopen ("/proc/meminfo", "r");
  bytes_read = fread (buffer, 1, BUFFERSIZE - 1, fp);
  fclose (fp);

/* Falla si la lectura fue erronea o si el buffer es pequeño */
  if (bytes_read == 0 || bytes_read == BUFFERSIZE - 1)
    {
      printf ("buffer pequeño");
      return;
    }

/* Finaliza el texto */
  buffer[bytes_read] = '\0';

/* Ubica la linea que comienza con “MemTotal”. */
  match = strstr (buffer, "MemTotal");

  if (match == NULL)
    {
      printf ("fallo lectura");
      return;
    }
  sscanf (match, "MemTotal: %i", &MemTotal);

  match = strstr (buffer, "MemFree");

  if (match == NULL)
    {
      printf ("fallo lectura");
      return;
    }

  sscanf (match, "MemFree: %i", &MemFree);

  printf ("Memoria total: %i kB \nMemoria Libre: %i kB \n", MemTotal, MemFree);
  return;

}

/**
 * @brief Numero files que soporta el kernel, el metodo cuenta desde el directorio /proc/filesystems
 * la cantidad de Files.
 * @param num_lineas contador que se incrementara por cada salto de linea en el file.
 */
void get_Num_FS ()
{
  FILE *fp;
  fp = fopen ("/proc/filesystems", "r");
  int num_lineas = 0;

/*
 * Leo todas las lineas del texto hasta encontrar el final, fgets devuelve NULL.
 */
  while (fgets (buffer, 1023, fp) != NULL)
    {
      num_lineas++;
    }
  fclose (fp);
  printf ("%i \n", num_lineas);
  return;
}

/**
 * @brief Imprime las estadisticas del disco, concretamente las solicitudes de escritura/lectura que
 * se realizaron. Dicha infomacion se extrae del directorio /proc/diskstats
 * @param diskstats se almacena el numero extraido del directorio.
 */
void get_diskstas ()
{
  FILE *fp;
  size_t bytes_read;
  char *match;
  int diskstats;

/* Lee el contenido dentro de /proc/diskstats y lo guarda en el buffer.*/
  fp = fopen ("/proc/diskstats", "r");
  bytes_read = fread (buffer, 1, BUFFERSIZE - 1, fp);
  fclose (fp);

/* Falla si la lectura fue erronea o si el buffer es pequeño */
  if (bytes_read == 0 || bytes_read == BUFFERSIZE - 1)
    return;

/* Finaliza el texto */
  buffer[bytes_read] = '\0';

/* Ubica la linea que comienza con “sda”. */
  match = strstr (buffer, "sda");

  if (match == NULL)
    return;

/* Analiza la linea y extrae el modelo */
  sscanf (match, "sda %*i %*i %*i %*i %i", &diskstats);
  printf ("%i \n", diskstats);
  return;
}

/**
 * @brief Se encarga de impirmir los cambios de contexto del sistema. Extrae dicha informacion desde
 * /proc/stat.
 * @param ctxt aqui se almacena el numero extraido.
 */
void context ()
{
  FILE *fp;
  size_t bytes_read;
  char *match;
  int ctxt;

/* Lee el contenido dentro de /proc/stat y lo guarda en el buffer.*/
  fp = fopen ("/proc/stat", "r");
  bytes_read = fread (buffer, 1, BUFFERSIZE - 1, fp);
  fclose (fp);

/* Falla si la lectura fue erronea o si el buffer es pequeño */
  if (bytes_read == 0 || bytes_read == BUFFERSIZE - 1)
    {
      printf ("Fallo");
      return;
    }

/* Finaliza el texto */
  buffer[bytes_read] = '\0';

/* Ubica la linea que comienza con “ctxt”. */
  match = strstr (buffer, "ctxt");

  if (match == NULL)
    {
      printf ("Fallo2");
      return;
    }
/* Analiza la linea y extrae el modelo */
  sscanf (match, "ctxt %i", &ctxt);

  match = strstr (buffer, "processes");

  if (match == NULL)
    {
      printf ("Fallo1");
      return;
    }

  sscanf (match, "ctxt %i", &ctxt);
  printf ("-Contex: %i \n", ctxt);
  return;
}

/**
 * @brief Metodo utilizado para la extraccion de la cantidad de procesos creados al inicio del sistema, extraido
 * del directorio /proc/stat
 * @param procread variable en la cual se almacena el numero de procesos.
 */
void Process ()
{

  FILE *fp;
  size_t bytes_read;
  char *match;
  int procread;

/* Lee el contenido dentro de /proc/stat y lo guarda en el buffer.*/
  fp = fopen ("/proc/stat", "r");
  bytes_read = fread (buffer, 1, BUFFERSIZE - 1, fp);
  fclose (fp);

/* Falla si la lectura fue erronea o si el buffer es pequeño */
  if (bytes_read == 0 || bytes_read == BUFFERSIZE - 1)
    {
      printf ("Fallo");
      return;
    }

/* Finaliza el texto */
  buffer[bytes_read] = '\0';

  match = strstr (buffer, "processes");

  if (match == NULL)
    {
      printf ("Fallo1");
      return;
    }

  sscanf (match, "processes %i", &procread);
  printf ("-Processes created: %i \n", procread);
  return;
}

/**
 * @brief Extrae el tiempo de actividad del sistema y calcula a que hora se encendio el sistema.
 * @param uptime aqui se almacena el tiempo en segundos desde que se inicio el sistema.
 */
void get_time ()
{
  time_t actual = time (NULL);
  FILE *fp;
  double uptime;
  fp = fopen ("/proc/uptime", "r");
  fscanf (fp, "%lf", &uptime);
  fclose (fp);
  actual = actual - (time_t) (uptime);
  printf ("Encendido: %s \n", asctime (localtime (&actual)));

}

/**
 * @brief Se encarga de imprimir los promedios de carga en 1 minuto, extrayendo dichos datos
 * desde el directorio /proc/loadavg
 * @param load variable utilizada para guardar el numero extraido.
 */
void LoadAvg ()
{
  FILE *fp;
  double load;
  fp = fopen ("/proc/loadavg", "r");
  fscanf (fp, "%lf", &load);
  fclose (fp);
  printf ("Lista de los promedios de carga en 1 minuto: %.2lf \n", load);
}

/**
 * @brief Nos imprime la informacion basica del sistema, como hostname, Date. A su vez hace
 * llamado a get_info_CPU, get_v_kernel, get_uptime y get_Num_FS.
 */
void printincio ()
{
  time_t inicio = time (NULL);
  FILE *fd;
  char buffer[BUFFERSIZE];
  fd = fopen ("/proc/sys/kernel/hostname", "r");
  fgets (buffer, BUFFERSIZE, fd);
  printf ("Hostname: %sDate: %s \n", buffer, asctime (localtime (&inicio)));
  fclose (fd);
  printf ("-Modelo y tipo de CPU: \n");
  get_info_CPU ();
  printf ("-Version de Kernel: \n");
  get_v_kernel ();
  get_uptime ();
  printf ("-Numero de FS: ");
  get_Num_FS ();
}
#endif
