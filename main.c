#include "hash.h"
#include "stdlib.h"

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