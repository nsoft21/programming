
# Принимает вес в килограммах и рост в метрах. Возвращает индекс массы тела.
def bmi(weight: float, height: float) -> float:
    mheight = height/100
    imt = weight / (mheight*mheight)
    return imt
# Принимает численное значение ИМТ и печатает на экран соответствующую категорию    
def print_bmi(bmi: float) -> float:
    if (bmi < 18.5):
        print("Underweight")
    elif (bmi < 25):
        print("Normal")
    elif (bmi < 30):
        print("Overweight")
    elif (bmi > 30):
        print("Obesity")
print("Введите вес в киллограммах и рост в сантиметрах через пробел:")
k = 0
while (k == 0):
    try:
        w, h = input().split()
        w = float(w)
        h = float(h)
        k = 1
    except:
        print("Never format")
imt = bmi(w,h)
print_bmi(imt)


