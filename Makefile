CFLAGS = -g -Wall
LDFLAGS = ar -rcs
CC = clang

all: libmlpt.a

libmlpt.a: mlpt.o mlpt.h
	$(LDFLAGS) libmlpt.a mlpt.o

mlpt.o: mlpt.c config.h mlpt.h 
	$(CC) $(CFLAGS) -c mlpt.c -o mlpt.o

clean:
	rm -f mlpt.o libmlpt.a 	


