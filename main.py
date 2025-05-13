from itertools import combinations

a = ["int", "float", "double", "char *", "void *"]

for i in range(len(a)):
    for j in range(i, len(a)):
        cur = f"typedef struct {{\n    {a[i]} key;\n    {a[j]} value;\n    int in_use;\n}} Entry;"
        print(cur)
        print()