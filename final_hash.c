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

typedef struct {
    void * key;
    void * val;
    int in_use;
} Entry;


typedef struct {
    int capa;
    int size;
    Entry *arr;
    uint32_t (*custom_hash)(void*);
    int (*custom_equals)(void*, void*);
} Map;


void map_init(Map *m, uint32_t (*custom_hash)(void *), int (*custom_equals)(void *, void *)) {
    m->capa = 16;
    m->size = 0;
    m->custom_hash = custom_hash;
    m->arr = calloc(m->capa, sizeof(Entry));
    if (!m->arr) {
        perror("error using malloc");
        exit(1);
    }
}


void map_add(Map *m, void *k, void *v) {
    // resize
    if (m->size * 4 >= m->capa * 3) {
        m->capa *= 2;
        Entry *arr_save = m->arr;
        m->arr = calloc(m->capa, sizeof(Entry));
        if (!m->arr) {
            free(arr_save);
            perror("error using malloc");
            exit(1);
        }
        m->size = 0;
        for (int i = 0; i < m->capa >> 1; i++) {
            map_add(m, arr_save[i].key, arr_save[i].val);
        }
        free(arr_save);
    }
    int hash = hash_spread(m->custom_hash(k));
    int index = hash & (m->capa - 1);
    while (m->arr[index].in_use == 1) {
        index = (index + 1) % m->capa;
    }
    m->arr[index] = (Entry){k, v, 1};
}

void *map_get(Map *m, void *k) {
    int hash = hash_spread(m->custom_hash(k));
    int index = hash & (m->capa - 1);
    for (int i = 0; i < m->capa; i++) {
        int cur_index = (index + i) % m->capa;
        if (m->arr[cur_index].in_use == 1 && m->custom_equals(m->arr[cur_index].key, k)) {
            return m->arr[cur_index].val;
        }
    }
    return NULL;
}