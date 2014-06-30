CC = gcc
CFLAGS = -g -pthread 

#LIBS = -lsocket -lnsl

all : schat cchat 

schat : schat.c 
	$(CC) $(CFLAGS) -o schat schat.c $(LIBS)

cchat : cchat.c
	$(CC) $(CFLAGS) -o cchat cchat.c $(LIBS)


clean: 
	rm -f cchat schat *.o *~