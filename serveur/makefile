CC=gcc
CFLAGS=
LDFLAGS=
EXEC=exe_serveur_ad

all: $(EXEC)

$(EXEC): serveur.o mainServeur.o
	$(CC) -o $(EXEC) serveur.o mainServeur.o $(LDFLAGS)

serveur.o: serveur.c struct.h
	$(CC) -o serveur.o -c serveur.c $(CFLAGS)

mainServeur.o: mainServeur.c serveur.h struct.h
	$(CC) -o mainServeur.o -c mainServeur.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
