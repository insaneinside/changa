# Change the N here to make a new size dataset.
N=1000

testdata.tbin: ppartt Makefile
	./ppartt $N -.5 .5 -1 > testdata.tbin

ppartt: ppartt.c tipsydefs.h
	$(CC) -o ppartt ppartt.c -lm

pgaspart: pgaspart.c tipsydefs.h
	$(CC) -o pgaspart pgaspart.c -lm

clean:
	-rm -f pgaspart ppartt testdata.tbin

