a = int(input("Введите a: "))
b = int(input("Введите b: "))
x = a
a = b
b = x
print(f"a = {a}", f"b = {b}", sep="\n")
a,b = b,a
print(f"a = {a}", f"b = {b}", sep="\n")
