def forward_pass(network, x):
	import math
	for layer in network:
		if type(layer)==list and layer[0][:7]=="linear_":
			i=0
			tmpl=[]
			while i<len(layer[1]):
				ii=0
				toplam=0
				while ii<len(layer[1][i]):
					toplam = toplam+x[ii]*layer[1][i][ii]
					ii+=1
				tmpl.append(toplam)
				i+=1
		
		elif layer[:4]=="relu":
			tmpl=[]
			for cur in x:
				if cur<0: sonhal=0
				else: sonhal=cur
				tmpl.append(sonhal)
		
		elif layer[:4]=="sigm":
			tmpl=[]
			for cur in x:
				if cur<=-700: sonhal=0
				elif cur<700: sonhal=1/(1+math.exp(-cur))
				else: sonhal=1
				tmpl.append(sonhal)
		x=tmpl
	return x
