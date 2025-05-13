#include "stdio.h"
#include "stdint.h"
#include "string.h"

uint32_t hashString(const char *s) {
    uint32_t h = 0;
    while (*s) {
        h = 31 * h + (unsigned char)(*s++);
    }
    return h;
}

uint32_t hashInt(const int a) {
    return a;
}

uint32_t hashFloat(const float a) {
    uint32_t h;
    memcpy(&h, &a, sizeof(float));
    return h;
}

uint32_t hashDouble(const double a) {
    uint64_t h;
    memcpy(&h, &a, sizeof(double));
    return (uint32_t)(h ^ (h >>> 32));
}

uint32_t hashCombine(uint32_t a, uint32_t b) {
    return a * 31 + b;
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


int main(void) {
    float a = 3.1415;
    printf("%u\n", hashFloat(a));
}