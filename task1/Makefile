all: test

CC := gcc
CFLAGS := -g -Wall -MD --coverage 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test: dinamicArray.o test.o
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