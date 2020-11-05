print('Введите целое число, затем два диапазона целых чисел')
try:
  s, l1, r1, l2, r2= map(int, input().split())
  if l1 + l2 == s:
    print(l1, l2)
  elif l1 + r2 == s:
    print(l1, r2)
  else:
    if r1 + l2 == s:
      print(r1, l2)
    elif r1 + r2 == s:
      print(r1, r2)
    else:
      print(-1)
except:
  print("Неверный формат ввода.")
