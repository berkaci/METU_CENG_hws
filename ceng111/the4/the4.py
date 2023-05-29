def cocukvarmi(kisi,agac,agaclis,oluler):
	olucocuklar=[]
	if kisi in agaclis:
		for a in agac[agaclis.index(kisi)]:
			if olumu(a,oluler): olucocuklar.append(a)
			else: return True
		k=False
		for a in olucocuklar:
			k= k or cocukvarmi(a,agac,agaclis,oluler)
		return k
	else: return False
	
def cocuklaradagit(kisi,para,agac,agaclis,oluler,miras,miraslis):
	bolunecek=0
	oludegilmi=[]
	cvm=[]
	indeks=agaclis.index(kisi)
	for a in agac[indeks]:
		oludegilmi.append(not olumu(a,oluler))
		cvm.append(cocukvarmi(a,agac,agaclis,oluler))
		if oludegilmi[-1] or cvm[-1]: bolunecek+=1
	i=0
	for a in agac[indeks]:
		if oludegilmi[i]:
			mirasekle(a,miras,miraslis,para/bolunecek)
		elif cvm[i]:
			cocuklaradagit(a,para/bolunecek,agac,agaclis,oluler,miras,miraslis)
		i+=1

def anabababul(kisi,agac,agaclis):
	ii=0
	ablist=[]
	for asdf in agac:
		if kisi in asdf: ablist.append(agaclis[ii])
		ii+=1
	return ablist
	
def olumu(kisi,oluler):
	return ["DEPARTED",kisi] in oluler
	
def mirasekle(kisi,miras,miraslis,para):
			if kisi in miraslis: 
				miras[miraslis.index(kisi)]+=para
			else:
				miraslis.append(kisi)
				miras.append(para)

def inheritance(Descriptions):
	cocuk=[]
	evli=[]
	olu=[]
	olen=[]
	treelis=[]
	tree=[]
	miras=[]
	miraslis=[]
	for srg in Descriptions:
		srl=list(filter(lambda asd: len(asd)!=0, srg.split(" ")))
		if srg.find("CHILD")!=-1: cocuk.append(srl)
		elif srg.find("MARRIED")!=-1 : evli.append(srl)
		elif srg.find("DEPARTED")!=-1 : olu.append(srl)
		elif srg.find("DECEASED")!=-1 : 
			olen.append(srl)
			olu.append(["DEPARTED",srl[1]])
	for srg in cocuk:
		if srg[1] in treelis: tree[treelis.index(srg[1])].extend(srg[3:])
		else: 
			treelis.append(srg[1])
			tree.append(srg[3:])
		if srg[2] in treelis: tree[treelis.index(srg[2])].extend(srg[3:])
		else: 
			treelis.append(srg[2])
			tree.append(srg[3:])
		
	for srg in olen:
		srg[2]=float(srg[2])
		esihayatta=False
		for esi in evli:
			if esi[1]==srg[1]:
				es=esi[2]
				esihayatta= not olumu(es,olu)
			elif esi[2]==srg[1]:
				es=esi[1]
				esihayatta=not olumu(es,olu)

		#pg1
		if cocukvarmi(srg[1],tree,treelis,olu):
			if esihayatta:
				mirasekle(es,miras,miraslis,srg[2]/4)
				srg[2]= srg[2]*3/4
			cocuklaradagit(srg[1],srg[2],tree,treelis,olu,miras,miraslis)
			#pg2
		else:
			abb=anabababul(srg[1],tree,treelis)
			bolen=0
			for ab in abb:
				if (not olumu(ab,olu)) or cocukvarmi(ab,tree,treelis,olu): bolen+=1
			if esihayatta and bolen!=0:
				mirasekle(es,miras,miraslis,srg[2]/2)
				srg[2]= srg[2]/2
			for ab in abb:
				if not olumu(ab,olu):
					mirasekle(ab,miras,miraslis,srg[2]/bolen)
				elif cocukvarmi(ab,tree,treelis,olu):
					cocuklaradagit(ab,srg[2]/bolen,tree,treelis,olu,miras,miraslis)
				#pg3
			if bolen==0:
				babb=[]
				for ab in abb:
					babb.append(anabababul(ab,tree,treelis))
				buyukbolen=0
				for ab in babb:
					for bab in ab:
						if (not olumu(bab,olu)) or cocukvarmi(bab,tree,treelis,olu): buyukbolen+=1
				if esihayatta and buyukbolen!=0:
					mirasekle(es,miras,miraslis,srg[2]*3/4)
					srg[2]= srg[2]/4
				for ab in babb:
					for bab in ab:
						if not olumu(bab,olu):
							mirasekle(bab,miras,miraslis,srg[2]/buyukbolen)
						elif cocukvarmi(bab,tree,treelis,olu):
							cocuklaradagit(bab,srg[2]/buyukbolen,tree,treelis,olu,miras,miraslis)
				if buyukbolen==0 and esihayatta: mirasekle(es,miras,miraslis,float(srg[2]))
	sonuc=[]
	iii=0
	for isim in miraslis:
		sonuc.append((isim,miras[iii]))
		iii+=1
		
	return sonuc
