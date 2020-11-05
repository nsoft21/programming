k = 0
print("Введите число:")
while (k == 0):
    try:
        a = int(input())
        k = 1
    except:
        print("Неверный формат ввода. Попробуйте еще раз:")
b = 1
while a > 1:
    b *= a
    a -= 1
if ((b < 1) or (b > 10**9)):
    print("Слишком большое число")
else:
    print(b)

