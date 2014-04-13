#!/usr/bin/python
fp = open('symbol.list','r')
ofp = open('LossdayConnsMaster.rpt','w')
for line in fp:
	line = line.strip().split()
	connFile='/data/work/2009/filtered/CurSymbol.'+line[0]+'.lossday.sorted.filtered'	
	print connFile
	cfp = open(connFile,'r')
	for li in cfp:
		li = li.strip().split(',')
		nline = line[0]+','+str(li[3])+','+str(li[0].replace(' ',''))+','+str(li[1])+','+str(li[2].replace(' ',''))+','+str(li[4])+','+str(li[5])
		ofp.write(nline+'\n')
	cfp.close()
ofp.close()
fp.close()
		
