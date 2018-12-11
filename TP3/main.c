/**
 * @author Martin Lujan
 * @date 11 diciembre 2018
 * @brief Implementacion de la funcion conocida "malloc", la cual se encarga de reservar una cierta cantidad
 * de memoria, dependiendo del argumento que esta reciba. Ademas, se ecuentra la implementacion de la funcion free
 * encargada de liberar los bloques de memoria.
 */
#include <stdio.h>
#include <zconf.h>
#include <string.h>

# define BLOCK_SIZE 48
# define align4(x) (((((x)-1)>>2)<<2)+4)


/* Bloque de meta data */
struct s_block {
    size_t size; //Tamaño del bloque
    struct s_block *next; //Proximo nodo
    struct s_block *prev; //Nodo anterior
    int free; //Si se encuentra libre o no
    void *ptr; //puntero a bloque alocado
    char data[1]; //Puntero al fin del bloque
};

typedef struct s_block *t_block;

void *base = NULL;

void split_block(t_block b, size_t s);

t_block find_block(t_block *last, size_t size);

t_block extend_heap(t_block last, size_t s);

t_block fusion(t_block b);

t_block get_block(void *p);

int valid_addr(void *p);

void free(void *p);

void *mallocFalse(size_t size);

int main() {
    /* Punteros a char e int para probar las funciones */
    int i;
    char *string; //4bytes un int

    string = (char *) mallocFalse(1);

    for (i = 1; i <= 10; i++) {
        *string = 'a';
        string++;
    }

    free(string);
    /*
    for (i; i <= 10; i++) {
        *string = 'a';
        string++;
    }*/
    return 0;
}

/**
 * Muevo el break pointer en "s" lugares, partiendo desde la posicion Last.
 * @param last puntero al ultimo break.
 * @param s tamaño que se quiere expandir.
 * @return un puntero a la estructura s_block
 */
t_block extend_heap(t_block last, size_t s) {
    t_block b;
    b = sbrk(0);

    if (sbrk(BLOCK_SIZE + s) == (void *) -1) {
        /* sbrk falla, devuelve NULL */
        return (NULL);
    }

    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data;
    if (last) {
        last->next = b;
    } else {
        b->free = 0;
    }

    return (b);
}

/**
 * Busca el bloque de memoeria necesario para otro bloque.
 * @param last Ultimo bloque visitado
 * @param size tamaño a expandir
 * @return puntero a la estructura s_block
 */
t_block find_block(t_block *last, size_t size) {
    t_block b = base;

    /* Mientras exista un bloque y no este libre, ni tenga suficiente memoria */
    while (b && !(b->free && b->size >= size)) {

        /* Indico el ultimo bloque visitado y me voy al siguiente */
        *last = b;
        b = b->next;
    }
    return (b);
}

/**
 * Se encarga de crear un nuevo bloque desplazado en "s lugares de la posicion actual "b".
 * @param b puntero a bloque.
 * @param s tamaño requerido.
 */
void split_block(t_block b, size_t s) {
    t_block new;

    /* Inicializo el nuevo bloque */
    new = (t_block) (b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->free = 1;
    new->ptr = new->data;

    /* Modifico el bloqu actual para que tenga en cuenta el nuevo bloque */
    b->size = s;
    b->next = new;
    if (new->next) {
        new->next->prev = new;
    }
}

/**
 * Se encarga de asignar memoria a nuestro programa a medida que se lo vallamos pidiendo.
 * @param size tamaño que se quiere expandir.
 * @return puntero a la posicion de memoria.
 */
void *mallocFalse(size_t size) {
    t_block b, last;
    size_t s;

    /* Alineado de punteros */
    s = align4(size);
    if (base) {
/* Primero encuentra el bloque */
        last = base;
        b = find_block(&last, s);
        if (b) {
/* Verificamos si podemos dividir */
            if ((b->size - s) >= (BLOCK_SIZE + 4))
                split_block(b, s);
            b->free = 0;
        } else {
/* No hay bloque de conexion, se extiende el heap */
            b = extend_heap(last, s);
            if (!b)
                return (NULL);
        }
    } else {

/* Primera vez */
        b = extend_heap(NULL, s);
        if (!b) {
            return (NULL);
        }
        base = b;
    }
    return (b->ptr);
}

/**
 * Realiza la fusion del bloque actual con el siguiente.
 * @param b puntero al bloque a fusionar.
 * @return puntero al bloque fusionado.
 */
t_block fusion(t_block b) {
    if (b->next && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next)
            b->next->prev = b;
    }
    return (b);
}

/**
 * Nos devuelve un puntero a la estructura con toda la informacion del bloque
 * @param p puntero a la direccion de memoria.
 * @return mismo puntero pero a la informacion del bloque.
 */
t_block get_block(void *p) {
    char *tmp;
    tmp = p;
    return (p = tmp - BLOCK_SIZE);
}

/**
 * Verifica si la direccion apunta a un bloque valido.
 * @param p puntero al bloque.
 * @return los datos del bloque, un cero si es invalido.
 */
int valid_addr(void *p) {
    if (base) {
        if (p > base && p < sbrk(0)) {
            return (p == (get_block(p))->ptr);
        }
    }
    return 0;
}

/**
 * Libera la memoria deseada.
 * @param p puntero a la direccion de memoria a liberar,
 */
void free(void *p) {
    t_block b;
    if (!valid_addr(p)) {
        b = get_block(p);
        b->free = 1;
/* Fusion con la anterior si es posible */
        if (b->prev && b->prev->free)
            b = fusion(b->prev);
/* Luego fusion con el siguiente */
        if (b->next)
            fusion(b);
        else {
/* Libera al final del heap */
            if (b->prev)
                b->prev->next = NULL;
            else
/* Si no hay previo es porq libere todos los bloques*/
                base = NULL;
            brk(b); //Borrado del bloque actual.
        }
    }
}