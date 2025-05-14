#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

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

int equals_int(void *a, void *b) {
    return *((int *)a) == *((int *)b);
}

int equals_string(void *a, void *b) {
    int res = strcmp((char *)a, (char *)b);
    return res == 0 ? 1 : 0;
}

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
    m->custom_equals = custom_equals;
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
            if (arr_save[i].in_use) {
                map_add(m, arr_save[i].key, arr_save[i].val);
            }
        }
        free(arr_save);
    }
    int hash = hash_spread(m->custom_hash(k));
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

void *map_get(Map *m, void *k, void *default_val) {
    int hash = hash_spread(m->custom_hash(k));
    int index = hash & (m->capa - 1);
    for (int i = 0; i < m->capa; i++) {
        int cur_index = (index + i) % m->capa;
        if (m->arr[cur_index].in_use == 1 && m->custom_equals(m->arr[cur_index].key, k)) {
            return m->arr[cur_index].val;
        }
    }
    return default_val;
}


const char *fruits[] = {
    "pumpkin", "pumpkin", "banana", "melon", "melon", "banana", "banana",
    "banana", "banana", "melon", "apple", "melon", "mango", "melon", "apple",
    "melon", "mango", "apple", "banana", "mango", "pumpkin", "banana", "melon",
    "pumpkin", "mango", "melon", "apple", "banana", "melon", "pumpkin", "pumpkin",
    "apple", "pumpkin", "banana", "banana", "banana", "banana", "apple", "apple",
    "apple", "pumpkin", "banana", "melon", "melon", "apple", "mango", "apple",
    "melon", "pumpkin", "melon", "mango", "melon", "apple", "pumpkin", "mango",
    "pumpkin", "melon", "apple", "banana", "melon", "pumpkin", "mango", "apple",
    "melon", "pumpkin", "apple", "mango", "pumpkin", "mango", "apple", "apple",
    "apple", "banana", "apple", "banana", "pumpkin", "mango", "mango", "melon",
    "banana", "banana", "apple", "melon", "banana", "pumpkin", "melon", "apple",
    "banana", "apple", "banana", "melon", "melon", "apple", "banana", "mango",
    "pumpkin", "apple", "melon", "apple", "banana"
};

int main() {
    Map test_map;
    map_init(&test_map, hash_string, equals_string);
    
    for (int i = 0; i < 100; i++) {
        int *default_val = calloc(1, sizeof(int));
        if (!default_val) {
            perror("error using malloc");
            return 1;
        }
        int *cur_val = (int *)map_get(&test_map, (void *)fruits[i], (void *)default_val);
        (*cur_val)++;
        map_add(&test_map, (void *)fruits[i], (void *)cur_val);
    }
    printf("%d\n", test_map.size);
    for (int i = 0; i < test_map.capa; i++) {
        Entry e = test_map.arr[i];
        if (e.in_use) {
            printf("%s: %d\n", (char*)e.key, *(int*)e.val);
        }
    }
}