x,v,t = input().split()
x=float(x)
v=float(v)
t=float(t)
#print(x,v,t, sep="\n")
a = 9.8
xt = x+v-((a*t**2)/2)
print(xt)
