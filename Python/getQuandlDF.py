import os
import Quandl
from time import sleep
from multiprocessing import Process
def getandsave(symbol):
	GETURL="GOOG/NASDAQ_"
	URL=GETURL+symbol
	data=Quandl.get(URL,authtoken="Xq3mura9xjsuVKspqT7N",trim_start="2014-01-01",trim_end="2014-03-31")
	data.to_csv('/data/Archives/2014/'+str(symbol)+'.csv')
	
IFP=open("/data/nasdaq.current","r")
for line in IFP:
	line = line.strip().split()
	print str(line[0])
	if not (os.path.exists('/data/Archives/2014/'+str(line[0])+'.csv')):
		p=Process(target=getandsave,args=(str(line[0]),))
		p.start()
		p.join()
		sleep(10)
	else:
		print 'skipping '+str(line[0])+'\n'
	
IFP.close()
