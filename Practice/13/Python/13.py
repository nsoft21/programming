k = 0
while (k == 0):
    try:
        a = int(input('Введите число: '))
        if a < 2 or a > 10**9:
            print('Вводимое число должно быть в пределах от 2 до 10^9 включительно!')
        else:
            k = 1
    except:
        print("Неверный формат ввода. Попробуйте еще раз:")

num = 2
while a % num != 0:
    num += 1
if num == a:
    print('Простое')
else:
    print('Составное')
