all: thread

CC := gcc
CFLAGS := -g -Wall -MD --coverage -pthread

%.o: %.c
	$(CC) $(CFLAGS) -c $<

thread: thread.o int_count.o
	$(CC) $(CFLAGS) -o $@ $^

-include *.d

clean: 
	rm -rf *.o *.d *.gcno *.s *.gcda *.gcov test

valgrind: test
	valgrind ./test

gcov: run_tests
	gcov -acdfu dinamicArray.c

run_tests: test
	./test