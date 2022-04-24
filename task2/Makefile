all: thread

CC := gcc
CFLAGS := -g -Wall -MD --coverage -pthread

%.o: %.c
	$(CC) $(CFLAGS) -c $<

thread: thread.o int_count.o thread_opt.o
	$(CC) $(CFLAGS) -o $@ $^ -lm 

-include *.d

clean: 
	rm -rf *.o *.d *.gcno *.s *.gcda *.gcov test

valgrind: test
	valgrind ./test

gcov: run_tests
	gcov -acdfu dinamicArray.c

run_tests: test
	./test