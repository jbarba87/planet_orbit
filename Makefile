CC=gcc
LIBS=-lm

program: v_inicial.o planetas.o
	gcc -o v_inicial v_inicial.c $(LIBS)
	gcc -o planetas planetas.c $(LIBS)

clean:
	rm -f v_inicial planetas *~ *.o *.txt

