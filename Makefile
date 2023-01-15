#Makefile
all:	beacon-flood

beacon-flood:	beacon-flood.o
	g++ -o beacon-flood	 beacon-flood.o -lpcap

beacon-flood.o:	ieee80211.h	radiotap.h	beacon-flood.cpp

clean:
	rm -f beacon-flood
	rm -f *.o