# Target library
objects := queue.o
lib := libuthread.a
STATIC := ar rcs
CC := gcc
CFLAGS := -Wall -Wextra -Werror
all: $(objects) $(lib) 

$(lib): queue.o
	$(STATIC) $@ $^

queue.o: queue.c  
	$(CC) $(CFLAGS) -c queue.c -o queue.o
clean:
	rm -f $(lib) $(objects)

