#include <stdio.h>
#include <zconf.h>
#include <string.h>

# define align4 (x)(((((x)-1)>>2)<<2)+4)
# define BLOCK_SIZE 20

struct s_block {
    size_t size;
    struct s_block *next;
    struct s_block *prev;
    int free;
    void *ptr;
    /* Puntero al bloque asignado */
    char data[1];
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
    char *string;
    int *array;
    void *test;

    string = (char *) mallocFalse(20);
    test = sbrk(0);
    if (string != NULL) {
        printf("Test realizado con exito\n");
        array = (int *) mallocFalse(20);
        if (array != NULL) {
            printf("2do Test realizado con exito\n");
        } else {
            printf("ERROR al alocar memoria 1\n");
        }
    } else {
        printf("ERROR al alocar memoria");
    }

    free(string);
    free(array);
    return 0;
}

t_block extend_heap(t_block last, size_t s) {
    t_block b;
    b = sbrk(0);
    if (sbrk(BLOCK_SIZE + s) == (void *) -1) {
/* sbrk falla, devuelve NULL */
        return (NULL);
    }
    b->size = s;
    b->next = NULL;
    if (last) {
        last->next = b;
    } else {
        b->free = 0;
    }

    return (b);
}

t_block find_block(t_block *last, size_t size) {
    t_block b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return (b);
}

void split_block(t_block b, size_t s) {
    t_block new;
    new = (t_block) (b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->free = 1;
    b->size = s;
    b->next = new;
}

void *mallocFalse(size_t size) {
    t_block b, last;
    size_t s;
    // s = align4(size);
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
        if (!b)
            return (NULL);
        base = b;
    }
    return (b->data);
}

t_block fusion(t_block b) {
    if (b->next && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next)
            b->next->prev = b;
    }
    return (b);
}

t_block get_block(void *p) {
    char *tmp;
    tmp = p;
    return (p = tmp -= BLOCK_SIZE);
}

/**
 * Verifica si la direccion libre es valida
 * @param p
 * @return
 */
int valid_addr(void *p) {
    if (base) {
        if (p > base && p < sbrk(0)) {
            return (p == (get_block(p))->ptr);
        }
    }
    return (0);
}

void free(void *p) {
    t_block b;
    if (valid_addr(p)) {
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
/* No mas bloques */
                base = NULL;
            brk(b);
        }
    }
}