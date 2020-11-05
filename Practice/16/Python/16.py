k = 0
while (k == 0):
    try:
        qnt = int(input('Введите количество билетов: '))
        k = 1
    except:
        print("Never format")
k = 0
print("Введите номера билетов:")
while (k == 0):
    try:
        tickets = input().split()
        list = []
        j = 0
        for i in tickets:
            list.append(i)
            j += 1
        if (j != qnt):
            print("Never format")
        else:
            k = 1
    except:
        print("Never format")

key = 'a55661'
num = 0
for ticket in tickets:
    key_ticket = ticket[0] + ticket[4:]
    if key_ticket == key:
        print(ticket)
        num += 1
if num == 0:
    print(-1)
