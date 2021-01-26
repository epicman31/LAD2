CC=gcc
CFLAGS=
LDFLAGS=
EXEC=exe_client_ad

all: $(EXEC)

$(EXEC): client.o mainClient.o
	$(CC) -o $(EXEC) client.o mainClient.o $(LDFLAGS)

client.o: client.c struct.h
	$(CC) -o client.o -c client.c $(CFLAGS)

mainClient.o: mainClient.c client.h struct.h
	$(CC) -o mainClient.o -c mainClient.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
