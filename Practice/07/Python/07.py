import math
print('''Выберите способ ввода параметров треугольника:
Введите 1 если хотите ввести длины сторон a, b, c;
Введите 2 если хотите ввести координаты вершин треугольника.''')
i = int(input('Формат ввода: '))
if i == 1:
    a = float(input('Длина стороны a = '))
    b = float(input('Длина стороны b = '))
    c = float(input('Длина стороны c = '))
    p = (a+b+c)/2
    S = math.sqrt(p*(p-a)*(p-b)*(p-c))
    print(f'S = {S}')
elif i == 2:
    x1, y1 = map(float, input('Координаты A через пробел:').split())
    x2, y2 = map(float, input('Координаты B через пробел:').split())
    x3, y3 = map(float, input('Координаты C через пробел:').split())
    A = abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0
    print(A)