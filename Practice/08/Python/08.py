print("Введите операцию для вычисления (число знак число):")
k = 0
while (k==0):
        try:
                a, b, c = input().split()
                a, c = float(a), float(c)
                if b in ('+', '-', '*', '/'):
                        if b == '+':
                                print(a + c)
                        elif b == '-':
                                print(a - c)
                        elif b == '*':
                                print(a * c)
                        elif b == '/':
                                if c != 0:
                                        print(a / c)
                                else:
                                        print("Деление на ноль!")
                        k = 1
                else:
                        print("Неверный знак! Введите '+', '-', '*', '/'")
        except:
                print("Неверный формат ввода. Попробуйте еще раз:")
