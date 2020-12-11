k = 0
while (k==0):
    try:
        num = int(input('Введите число: '))
        k = 1
    except:
        print("Неверный формат ввода.\n")

arr = {}
while num > 1:
    for i in range(2, num + 1):
        if num % i == 0:
            num //= i
            arr[i] = arr.get(i, 0) + 1
            break

quant = len(arr.keys())
i = 0
for k, v in arr.items():
    i += 1
    print(k, end='')
    if v > 1:
        print(f'^{v}', end='')
    if i != quant:
        print('*', end='')