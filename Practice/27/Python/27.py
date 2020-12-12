k = 0
while (k==0):
    try:
        n = int(input('Введите кол-во эл-ов: '))
        arr = map(int, input('Введите эл-ты массива через пробел: ').split())
        k = 1
    except:
        print("Неверный формат ввода.\n")

list1 = []
for el in arr:
    for i in range(6):
        if i >= len(list1):
            break
        elif el > list1[i]:
            list1.insert(i, el)
            break

    if i == 5 or i == len(list1):
        list1.append(el)
    if len(list1) > 5:
        list1.pop(0)

    for el in list1:
        print(el, end=' ')
    print()
