import math
from array import *
print("Введите количество денег:")
k = 0
while (k == 0):
    money = input()
    try:
        money=int(money)
        k = 1
    except:
        print("Введите целое число!")
print("Введите количество напитков:")
k = 0
while (k == 0):
    qdrink = input()
    try:
        qdrink=int(qdrink)
        k = 1
    except:
        print("Введите целое число!")
print("Введите название напитка|цену|объем через пробел:")
d = 0
k = 0
drinks = []
while (d < qdrink):
    k = 0
    while (k == 0):
        try:
            d_name, d_price, d_volume = input().split()
            d_name=str(d_name)
            d_price=int(d_price)
            d_volume=int(d_volume)
            if ((d_price > 0)and(d_volume>0)):
                list = [d_name,d_price,d_volume]
                drinks.append(list)
                d = d + 1
                k = 1
            else:
                print("Введите положительные значения!")
        except:
            print("Неверный формат ввода!")
try:
    b = 0
    c = -1
    s = []
    for i in drinks:
        botl = money//i[1]
        lit = botl*i[2]
        mon = money - botl * i[1]
        list = [i[0],botl,lit,mon]
        s.append(list)
        if (lit>b):
            print(lit)
            b = lit
            c = c + 1
    if (c < 0):
        print("-1")
    else:
        result = s[c]
        print(c)
        print(str(result[0]) + " " + str(result[1]))
        print(result[2])
        print(result[3])
except:
    print("Что-то пошло не так..")
#print(drinks)

