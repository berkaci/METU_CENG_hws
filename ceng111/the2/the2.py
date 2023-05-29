a=input()
b=list(a)
if b[5]=="X": b[5]="10"

if a[0]=='?':
	modonbir=((int(b[5])-(int(b[1])*3+int(b[2])*5+int(b[3])*7))%11)
	print(str((modonbir%2*11+modonbir)//2)+a[1:])
elif a[1]=="?":
	modonbir=((int(b[5])-(int(b[0])*2+int(b[2])*5+int(b[3])*7))%11)
	print( a[0]+str(((((((-(modonbir-1)%3-2+modonbir)//3+((-(modonbir-1)%3-2+modonbir)//3-modonbir)%2)*3-modonbir)//2)*11+modonbir)//3)%11)+a[2:])
elif a[2]=="?":
	modonbir=((int(b[5])-(int(b[0])*2+int(b[1])*3+int(b[3])*7))%11)
	modbes=(modonbir-1)%5+1
	print( a[:2]+str((((5-modbes)*11+modonbir)//5)%11)+a[3:])
elif a[3]=="?":
	modonbir=((int(b[5])-(int(b[0])*2+int(b[1])*3+int(b[2])*5))%11)
	moduc=(modonbir-1)%3+1
	k=(7*((4*(3-moduc)+modonbir)//3)-modonbir)//4
	print( a[:3]+str(((11*k+modonbir)//7)%11)+a[4:])
elif a[5]=="?":
	l=(((int(b[0])*2+int(b[1])*3+int(b[2])*5+int(b[3])*7)%11))
	if l==10: l="X"
	print( a[:5]+str(l))
elif (int(b[0])*2+int(b[1])*3+int(b[2])*5+int(b[3])*7)%11==int(b[5]):
	print("VALID")
else:
	print("INVALID")
