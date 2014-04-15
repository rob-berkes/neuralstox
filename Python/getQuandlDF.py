import Quandl
GETURL="GOOG/NASDAQ_"
COUNTGOOD=0
COUNTBAD=0
IFP=open("SYMBOLS.NASDAQ","r")
for line in IFP:
	line = line.strip().split()
	URL=GETURL+line[0]
	try:
		data=Quandl.get(URL,authtoken="Xq3mura9xjsuVKspqT7N",trim_start="2014-01-01",trim_end="2014-03-31")
		data.to_csv('/data/Archives/2014/'+str(line[0])+'.csv')
		time.sleep(0.3)
		print line,
		print " recorded to file.\n"
		COUNTGOOD+=1
	except: 
		COUNTBAD+=1
IFP.close()
print COUNTGOOD,
print " good records written ",
print COUNTBAD,
print " bad records skipped."
	
