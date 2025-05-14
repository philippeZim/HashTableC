a = [
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
]

d = dict()

for x in a:
    d[x] = d.get(x, 0) + 1
print(len(d))
for k, v in d.items():
    print(k, v)