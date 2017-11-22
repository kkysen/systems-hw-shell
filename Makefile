CC = gcc -ggdb -std=c99 -Wall -Werror -O3
OUT = shell

util.o:
	$(CC) -c util.c

stacktrace.o:
	$(CC) -c stacktrace.c

argv.o:
	$(CC) -c argv.c

builtins.o:
	$(CC) -c builtins.c

cd.o:
	$(CC) -c cd.c

aliases.o:
	$(CC) -c aliases.c

$(OUT).o:
	$(CC) -c $(OUT).c

all: clean $(OUT).o util.o stacktrace.o argv.o builtins.o cd.o aliases.o
	$(CC) -o $(OUT) $(OUT).o util.o stacktrace.o argv.o builtins.o cd.o aliases.o

clean:
	rm -f *.o
	rm -f $(OUT)
	rm -f test_argv

install: clean all

run: install
	./$(OUT)

rerun: all
	./$(OUT)

valgrind: clean all
	valgrind -v --leak-check=full ./$(OUT)

test_argv.o:
	$(CC) -c test_argv.c

test_argv: test_argv.o argv.o stacktrace.o
	$(CC) -o test_argv test_argv.o argv.o stacktrace.o
	./test_argv

test: clean test_argv