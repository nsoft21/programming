a, b, c = input(': ').split()
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
else:
    print("Неверный знак!")