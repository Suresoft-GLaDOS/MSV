CC := msv-profilecc

all:
	$(CC) -c test.c $(CFLAGS) -o test.o
	$(CC) test.o $(CFLAGS) -o prog

clean:
	rm -f prog
	rm -f test.o
