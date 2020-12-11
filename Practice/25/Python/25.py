from math import *
from random import randrange

def bozo(arr, direction=True):
    narr = []
    if isinstance(arr[0], list):
        for li in arr:
            narr.extend(li)
    else:
        narr = arr.copy()

    len_narr = len(narr)
    while not sort(narr, direction):
        first, second = randrange(len_narr), randrange(len_narr)
        narr[first], narr[second] = narr[second], narr[first]
    return narr

def sort(arr, direction=True):
    arr_len = len(arr)
    if direction:
        for i in range(1, arr_len):
            if arr[i - 1] > arr[i]:
                return False
    else:
        for i in range(1, arr_len):
            if arr[i - 1] < arr[i]:
                return False
    return True

i = 0
k = 0
while (k==0):
    try:
        length = int(input('Введите число: '))
        sqrt_len = sqrt(length)
        list1 = [int(i) for i in input('Введите эл-ты массива через пробел: ').split()]
        k = 1
    except:
        print("Неверный формат ввода.\n")

list2 = []
list3 = []

for el in list1:
    list3.append(el)
    i += 1
    if i == sqrt_len:
        list2.append(list3)
        i = 0
        list3 = []
del list3, i

els = []
quant = 3
for i in range(quant):
    els.append(list1[i])

print(' '.join(map(str, bozo(list1, True))))
print(' '.join(map(str, bozo(list1, False))))
print(' '.join(map(str, bozo(list2, True))))
print(' '.join(map(str, bozo(list2, False))))
print(' '.join(map(str, bozo(els, True))))
print(' '.join(map(str, bozo(els, False))))