# Target library
objects := queue.o uthread.o context.o sem.o
lib := libuthread.a
STATIC := ar rcs
CC := gcc
CFLAGS := -Wall -Wextra -Werror
all: $(objects) $(lib) 

$(lib): $(objects)
	$(STATIC) $@ $^


queue.o: queue.c  
	$(CC) $(CFLAGS) -c queue.c -o queue.o

uthread.o: uthread.c 
	$(CC) $(CFLAGS) -c uthread.c -o uthread.o

context.o: context.c
	$(CC) $(CFLAGS) -c context.c -o context.o

sem.o: sem.c
	$(CC) $(CFLAGS) -c sem.c -o sem.o
clean:
	rm -f $(lib) $(objects)

