CC = gcc -ggdb -std=gnu11 -Wall -Werror -O3 -pthread
OUT = shell

generate_objects:
	python3 generate_makefile.py

make: generate_objects

# start generate_objects()

aliases.o: 
	$(CC) -c aliases.c -o aliases.o

argvv.o: 
	$(CC) -c argvv.c -o argvv.o

chars.o: 
	$(CC) -c chars.c -o chars.o

preprocessing.o: 
	$(CC) -c preprocessing.c -o preprocessing.o

run_commands.o: 
	$(CC) -c run_commands.c -o run_commands.o

shell.o: 
	$(CC) -c shell.c -o shell.o

shell_io.o: 
	$(CC) -c shell_io.c -o shell_io.o

shell_utils.o: 
	$(CC) -c shell_utils.c -o shell_utils.o

builtins/alias.o: 
	$(CC) -c builtins/alias.c -o builtins/alias.o

builtins/builtins.o: 
	$(CC) -c builtins/builtins.c -o builtins/builtins.o

builtins/cd.o: 
	$(CC) -c builtins/cd.c -o builtins/cd.o

builtins/exit.o: 
	$(CC) -c builtins/exit.c -o builtins/exit.o

builtins/pid.o: 
	$(CC) -c builtins/pid.c -o builtins/pid.o

builtins/source.o: 
	$(CC) -c builtins/source.c -o builtins/source.o

builtins/unalias.o: 
	$(CC) -c builtins/unalias.c -o builtins/unalias.o

test/test_argv.o: 
	$(CC) -c test/test_argv.c -o test/test_argv.o

test/test_string_builder.o: 
	$(CC) -c test/test_string_builder.c -o test/test_string_builder.o

test/test_str_utils.o: 
	$(CC) -c test/test_str_utils.c -o test/test_str_utils.o

util/io_utils.o: 
	$(CC) -c util/io_utils.c -o util/io_utils.o

util/pipes.o: 
	$(CC) -c util/pipes.c -o util/pipes.o

util/semaphores.o: 
	$(CC) -c util/semaphores.c -o util/semaphores.o

util/shared_locks.o: 
	$(CC) -c util/shared_locks.c -o util/shared_locks.o

util/shared_memory.o: 
	$(CC) -c util/shared_memory.c -o util/shared_memory.o

util/stacktrace.o: 
	$(CC) -c util/stacktrace.c -o util/stacktrace.o

util/string_builder.o: 
	$(CC) -c util/string_builder.c -o util/string_builder.o

util/str_utils.o: 
	$(CC) -c util/str_utils.c -o util/str_utils.o

util/utils.o: 
	$(CC) -c util/utils.c -o util/utils.o

all: clean aliases.o argvv.o chars.o preprocessing.o run_commands.o shell.o shell_io.o shell_utils.o builtins/alias.o builtins/builtins.o builtins/cd.o builtins/exit.o builtins/pid.o builtins/source.o builtins/unalias.o util/io_utils.o util/pipes.o util/semaphores.o util/shared_locks.o util/shared_memory.o util/stacktrace.o util/string_builder.o util/str_utils.o util/utils.o
	$(CC) -o $(OUT) aliases.o argvv.o preprocessing.o run_commands.o shell.o shell_io.o shell_utils.o builtins/alias.o builtins/builtins.o builtins/cd.o builtins/exit.o builtins/pid.o builtins/source.o builtins/unalias.o util/io_utils.o util/pipes.o util/semaphores.o util/shared_locks.o util/shared_memory.o util/stacktrace.o util/string_builder.o util/str_utils.o util/utils.o
	make chars

# end generate_objects()

clean:
	touch dummy.o
	find . -name '*.o' -delete
	rm -f $(OUT)
	rm -f test_*

install: clean all

run: install
	./$(OUT)

rerun: all
	./$(OUT)

valgrind: clean all
	valgrind -v --leak-check=full ./$(OUT)

test_argv: test/test_argv.o util/stacktrace.o
	$(CC) -o test_argv test/test_argv.o test/stacktrace.o
	./test_argv

test_str_utils: test/test_str_utils.o util/str_utils.o util/stacktrace.o
	$(CC) -o test_str_utils test/test_str_utils.o util/str_utils.o util/stacktrace.o
	./test_str_utils

test_sb: test/test_string_builder.o util/string_builder.o util/stacktrace.o
	$(CC) -o test_sb test/test_string_builder.o util/string_builder.o util/stacktrace.o
	valgrind ./test_sb

chars: chars.o
	$(CC) -o chars chars.o

test: clean test_sb
