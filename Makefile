CC = gcc

all: main.c
	$(CC) main.c hashtable.c memcheck.c -o memchecker

clean:
	$(RM) memchecker