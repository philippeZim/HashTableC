#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

uint32_t hash_string(const char *s) {
    uint32_t h = 0;
    while (*s) {
        h = 31 * h + (unsigned char)(*s++);
    }
    return h;
}

uint32_t hash_int(const int a) {
    return a;
}

uint32_t hash_float(const float a) {
    uint32_t h;
    memcpy(&h, &a, sizeof(float));
    return h;
}

uint32_t hash_double(const double a) {
    uint64_t h;
    memcpy(&h, &a, sizeof(double));
    return (uint32_t)(h ^ (h >> 32));
}

uint32_t hash_combine(uint32_t a, uint32_t b) {
    return a * 31 + b;
}

uint32_t hash_spread(uint32_t h) {
    return h ^ (h >> 16);
}

/*
typedef struct {
    const char *name;
    int age;
    float height;
} Person;
 
You'd write the hash function as:

uint32_t hashPerson(const Person *p) {
    uint32_t h = 0;
    h = hashCombine(h, hashString(p->name));
    h = hashCombine(h, hashInt(p->age));
    h = hashCombine(h, hashFloat(p->height));
    return h;
}

*/

typedef enum {
    INT,
    FLOAT,
    DOUBLE,
    STRUCT,
    CHAR,
} Type;

typedef struct {
    Type key_type;
    Type val_type;
    int capa;
    int size;
    int *entry_sizes;
    int **int_arr;
    double **double_arr;
    float **float_arr;
    char ***char_arr;
    void **struct_arr;
    int (*custom_hash)(void *, void *)
} Map;





void map_init(Map *m, Type key_type, Type val_type, int (*custom_hash)(void *, void *)) {
    m->key_type = key_type;
    m->val_type = val_type;
    m->capa = 16;
    m->size = 0;
    m->custom_hash = custom_hash;
    m->entry_sizes = calloc(m->capa, sizeof(int));
    if (m->entry_sizes == NULL) {
        perror("error using malloc");
        exit(1);
    }
    if (val_type == INT) {
        m->int_arr = malloc(sizeof(int *) * m->capa);
        if (m->int_arr == NULL) {
            perror("error using malloc");
            exit(1);
        }
    } else if (val_type == FLOAT) {
        m->float_arr = malloc(sizeof(float *) * m->capa);
        if (m->float_arr == NULL) {
            perror("error using malloc");
            exit(1);
        }
    } else if (val_type == DOUBLE) {
        m->double_arr = malloc(sizeof(double *) * m->capa);
        if (m->double_arr == NULL) {
            perror("error using malloc");
            exit(1);
        }
    } else if (val_type == CHAR) {
        m->char_arr = malloc(sizeof(char **) * m->capa);
        if (m->char_arr == NULL) {
            perror("error using malloc");
            exit(1);
        }
    } else {
        m->struct_arr = malloc(sizeof(void *) * m->capa);
        if (m->struct_arr == NULL) {
            perror("error using malloc");
            exit(1);
        }
    }

}

void map_add(Map *m, void *k, void *v) {
    // resize
    if (m->size + 1 > m->capa * 0.75) {
        m->capa *= 2;
        if (m->val_type == INT) {
            m->int_arr = realloc(m->int_arr, sizeof(int *) * m->capa);
            if (m->int_arr == NULL) {
                perror("error using malloc");
                exit(1);
            }
        } else if (m->val_type == FLOAT) {
            m->float_arr = realloc(m->float_arr, sizeof(float *) * m->capa);
            if (m->float_arr == NULL) {
                perror("error using malloc");
                exit(1);
            }
        } else if (m->val_type == DOUBLE) {
            m->double_arr = realloc(m->double_arr, sizeof(double *) * m->capa);
            if (m->double_arr == NULL) {
                perror("error using malloc");
                exit(1);
            }
        } else if (m->val_type == CHAR) {
            m->char_arr = realloc(m->char_arr, sizeof(char **) * m->capa);
            if (m->char_arr == NULL) {
                perror("error using malloc");
                exit(1);
            }
        } else {
            m->struct_arr = realloc(m->struct_arr, sizeof(void *) * m->capa);
            if (m->struct_arr == NULL) {
                perror("error using malloc");
                exit(1);
            }
        }
    }
    if (m->key_type == INT) {
        int hash = hash_spread(hash_int(*((int *)k)));
        int index = hash & (m->capa - 1);

    } else if (m->key_type == FLOAT) {
    } else if (m->key_type == DOUBLE) {
    } else if (m->key_type == CHAR) {
    } else {
    }
}


int main(void) {
    float a = 3.1415;
    printf("%u\n", hashFloat(a));
    printf("%d\n", sizeof(double **));
}



/*
if (val_type == INT) {
} else if (val_type == FLOAT) {
} else if (val_type == DOUBLE) {
} else if (val_type == CHAR) {
} else {
}
*/