#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Hashing function types
typedef uint32_t (*hash_func)(void*);
typedef int (*equals_func)(void*, void*);

// Hash map entry
typedef struct {
    void *key;
    void *val;
    int in_use;
} Entry;

// Hash map structure
typedef struct {
    int capa;
    int size;
    Entry *arr;
    hash_func custom_hash;
    equals_func custom_equals;
} Map;

// Hash functions
uint32_t hash_string(void *v_s);
uint32_t hash_int(void *a);
uint32_t hash_float(void *a);
uint32_t hash_double(void *a);
uint32_t hash_combine(uint32_t a, uint32_t b);
uint32_t hash_spread(uint32_t h);

// Equality functions
int equals_int(void *a, void *b);
int equals_float(void *a, void *b);
int equals_double(void *a, void *b);
int equals_string(void *a, void *b);

// Map operations
void map_init(Map *m, hash_func custom_hash, equals_func custom_equals);
void map_add(Map *m, void *k, void *v);
void* map_get(Map *m, void *k, void *default_val);
void map_free(Map *m);

#endif // HASH_H