k = 0
while (k == 0):
    try:
        a = float(input('Введите число: '))
        b = int(input('Введите степень: '))
        k = 1
    except:
        print("Неверный формат ввода. Попробуйте еще раз:")
i = 0
c = 1
while i < b:
    c *= a
    i += 1
print(c)
