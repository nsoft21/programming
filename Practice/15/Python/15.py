from random import randint
k = 1
while (k == 1):
    a = randint(0, 100)
    i = 0
    print("Загадано число от 0 до 100, угадай с 5ти попыток:")
    while (i < 5):
        try:
            b = int(input())
        except:
            print("Введите целое число")
        if b < a:
            print('Загаданное число больше')
            i += 1
        elif b > a:
            print('Загаданное число меньше')
            i += 1
        elif b == a:
            print('Поздравляю! Вы угадали')
            i = 6
            print('Хотите начать сначала? (1 - ДА )')
            try:
                k = int(input())
            except:
                quit()
    if (i == 5):    
        print(f'Вы проиграли. Было загадано: {a}')
        print('Хотите начать сначала? (1 - ДА )')
        try:
            k = int(input())
        except:
            quit()
