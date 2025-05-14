#include "hash.h"
#include <stdlib.h>
#include <string.h>

// Hash functions

uint32_t hash_string(void *v_s) {
    char *s = (char *) v_s;
    uint32_t h = 0;
    while (*s) {
        h = 31 * h + (unsigned char)(*s++);
    }
    return h;
}

uint32_t hash_int(void *a) {
    return *((int *)a);
}

uint32_t hash_float(void *a) {
    uint32_t h;
    memcpy(&h, a, sizeof(float));
    return h;
}

uint32_t hash_double(void *a) {
    uint64_t h;
    memcpy(&h, a, sizeof(double));
    return (uint32_t)(h ^ (h >> 32));
}

uint32_t hash_combine(uint32_t a, uint32_t b) {
    return a * 31 + b;
}

uint32_t hash_spread(uint32_t h) {
    return h ^ (h >> 16);
}

/*
Example hash for a struct:

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

// Equality functions

int equals_int(void *a, void *b) {
    return *((int *)a) == *((int *)b);
}

int equals_float(void *a, void *b) {
    return *((float *)a) == *((float *)b);
}

int equals_double(void *a, void *b) {
    return *((double *)a) == *((double *)b);
}

int equals_string(void *a, void *b) {
    int res = strcmp((char *)a, (char *)b);
    return res == 0 ? 1 : 0;
}

// Map operations

void map_init(Map *m, hash_func custom_hash, equals_func custom_equals) {
    m->capa = 16;
    m->size = 0;
    m->custom_hash = custom_hash;
    m->custom_equals = custom_equals;
    m->arr = calloc(m->capa, sizeof(Entry));
    if (!m->arr) {
        perror("error using calloc");
        exit(1);
    }
}

void map_add(Map *m, void *k, void *v) {
    // Resize if needed
    if (m->size * 4 >= m->capa * 3) {
        m->capa *= 2;
        Entry *arr_save = m->arr;
        m->arr = calloc(m->capa, sizeof(Entry));
        if (!m->arr) {
            free(arr_save);
            perror("error using calloc");
            exit(1);
        }
        m->size = 0;
        for (int i = 0; i < m->capa / 2; i++) {
            if (arr_save[i].in_use) {
                map_add(m, arr_save[i].key, arr_save[i].val);
            }
        }
        free(arr_save);
    }

    uint32_t hash = hash_spread(m->custom_hash(k));
    int index = hash & (m->capa - 1);

    while (m->arr[index].in_use) {
        if (m->custom_equals(m->arr[index].key, k)) {
            m->size--;
            break;
        }
        index = (index + 1) % m->capa;
    }

    m->arr[index] = (Entry){k, v, 1};
    m->size++;
}

void* map_get(Map *m, void *k, void *default_val) {
    uint32_t hash = hash_spread(m->custom_hash(k));
    int index = hash & (m->capa - 1);

    for (int i = 0; i < m->capa; i++) {
        int cur_index = (index + i) % m->capa;
        if (m->arr[cur_index].in_use && m->custom_equals(m->arr[cur_index].key, k)) {
            return m->arr[cur_index].val;
        }
    }
    return default_val;
}

void map_free(Map *m) {
    if (m && m->arr) {
        free(m->arr);
        m->arr = NULL;
    }
    m->capa = 0;
    m->size = 0;
}