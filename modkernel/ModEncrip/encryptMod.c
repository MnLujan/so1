#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#define DEVICE_NAME "encrypt" //El disp se encontrara en /dev/encrypt
#define CLASS_NAME "encryptMod"      //Controlador de disp de caracteres ¿?

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple modulo de encriptacion para una cadena string");
static int Num;
static char string[256] = {0};
static short size_string;
static int NumOpens = 0; //cantidad de veces abierto el disp.
static struct class* encryptClass = NULL; 
static struct device* encryptDevice = NULL; 

/* Funciones para el tratamiento del dispositivo */
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/* Funciones de prototipo para el controlador */
static struct file_operations fops =
{
 .open = dev_open,
 .read = dev_read,
 .write = dev_write,
 .release = dev_release,
};

/**
 * Inicializacion del modulo
 */
static int __init encrypt_init(void){
  printk(KERN_INFO "encrypt: Inicializacion\n");
  
  /* Registro del major number */
  Num = register_chrdev(0,DEVICE_NAME,&fops);
  if(Num < 0){
    printk(KERN_ALERT "Falla al registrar major number\n");
    return Num;
  }
  printk(KERN_INFO "Registro correcto, con major number %d\n",Num);
    
   /* Se registra la clase de dispositivo */
   encryptClass = class_create(THIS_MODULE, CLASS_NAME);
   if(IS_ERR(encryptClass)){
       class_destroy(encryptClass);
       printk(KERN_ALERT "Fallo al crear el dispositivo\n");
       return PTR_ERR(encryptDevice);
       }
      printk(KERN_INFO "encrypt: clase de dispositivo creada\n");

    /* Registro del controlador */
    encryptDevice = device_create(encryptClass,NULL,MKDEV(Num,0),NULL,DEVICE_NAME);
    
    if(IS_ERR(encryptDevice))
    {
	   class_destroy(encryptClass);
	   unregister_chrdev(Num, DEVICE_NAME);
	   printk(KERN_ALERT "Falla al registrar dispositivo\n");
	   return PTR_ERR(encryptDevice);
	 }
	 else
	 {
       printk(KERN_INFO "encrypt: Dispositivo creado correctamente\n");
     }
   return 0;
}

/**
 * Finalizacion del modulo.
 */
 static void __exit encrypt_exit(void){
   device_destroy(encryptClass, MKDEV(Num,0));
   class_unregister(encryptClass);
   class_destroy(encryptClass);
   printk(KERN_INFO "Hasta la vista baby\n");
 }
 
 /**
  *  Funcion de apertura del dispositivo que se llama cada vez 
  *  que se abre el archivo.
  **/
 static int dev_open(struct inode *inodep, struct file *filep){
	 NumOpens++;
	 printk(KERN_INFO "Archivo a sido abierto %d veces\n",NumOpens);
	 return 0;
}

/**
 * Funcion que se ejecuta siempre que el usuario quiera leer.
 * 
 **/
 static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offsetof){
   int error;
   /* utiliza el formato (destino, fuente, numero de bytes), devuelve 0 si es exitoso */
   sprintf(buffer, "%s", string);
   error = copy_to_user(buffer, string, size_string);
   if(error == 0)
    {
	printk(KERN_INFO "Se copiaron %d caracteres al usuario\n",size_string);
	return(size_string = 0); //Se lipia la posicion y se retorna.  
    }
    else
     {
	 printk(KERN_INFO "Fallo la copia de %d caracteres del user\n", size_string);
	 return -EFAULT; //Error que indica la direccion de memoria invalida
	 }
 }
 
 /**
  * Funcion que se ejecuta cuando el usuario se encuentra escribiendo.
  **/
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
 int i;
 sprintf(string, "%s", buffer);
 size_string = strlen(string);
 for(i=0;i<len;i++){
 string[i] = string[i] + 2;
 }
 printk(KERN_INFO "Lo que se encripto fue: %s\n", string);
 return len;
}

/**
 * Funcion que se ejecuta cuando se libera el dispositivo desde el espacio
 * del usuario.
 **/
 static int dev_release(struct inode *inodep, struct file *filep){
  printk(KERN_INFO "Dispositivo cerrado con exito\n");
  return 0;
 }
 
 module_init(encrypt_init);
 module_exit(encrypt_exit);
 
 
 

 
 


