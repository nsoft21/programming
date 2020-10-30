import math
print("Введите числа a, b и c:")
k = 0
while (k == 0):
    a,b,c = input().split()
    try:
        a=float(a)
        b=float(b)
        c=float(c)
        k = 1
    except:
        print("Введите числа!")

if (a == 0):
    try:
        x1 = -c / b
    except:
        x1 = 0
    print("x = " + str(x1))
else:
    d = b * b - 4 * a * c
    if (d > 0):
        x1 = (-b + math.sqrt(d)) / (2 * a)
        x2 = (-b - math.sqrt(d)) / (2 * a)
        if (x1 == x2):
            print("x = " + str(x1))
        else:
            print("x1 = " + str(x1) + "; x2 = " + str(x2))
    else:
        print("Действительных корней нет!")

