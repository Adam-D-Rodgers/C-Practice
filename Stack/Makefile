CC = gcc -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wextra -Wpedantic -Werror -g


all: push_pop

push_pop: push_pop.o stack_status.o
	$(CC) -o push_pop push_pop.o stack_status.o

push_pop.o: push_pop.c push_pop.h
	$(CC) -c push_pop.c

stack_status.o: stack_status.c push_pop.h
	$(CC) -c stack_status.c
clean cls:
	rm -f *.o push_pop *~ \#*
