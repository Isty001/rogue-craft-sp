SRC = $(shell find src lib/*/src config -name '*.c')
TEST_SRC = $(SRC) $(shell find test -name '*.c')
CFLAGS = -std=c11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -I lib/mem-pool/src -I lib/worker/src -l ncursesw -l pthread -l m -isystem lib -D _GNU_SOURCE


.PHONY: test


run-debug:
	$(CC) $(SRC) main.c $(CFLAGS) -gdwarf -D DEBUG_MODE -o rogue.o
	./rogue.o

test:
	$(CC) $(TEST_SRC) $(CFLAGS) -D UNIT_TEST -D DEBUG_MODE -o test.o
	./test.o

test-valgrind:
	make test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./test.o
