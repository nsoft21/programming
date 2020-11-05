k = 0
print("Введите целое число:")
while (k == 0):
    try:
        a = int(input())
        if a < 0 or a > 10 ** 15:
            print('Число должно быть в промежутке от 0 до 10^15 включительно!')
        else:
            k = 1
    except:
        print("Неверный формат. Попробуйте еще раз:")
b = 0
c = 1
while c <= a:
    c *= 2
    b += 1
print(b)
