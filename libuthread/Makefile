# Target library
objects := queue.o
lib := libuthread.a
STATIC := ar rcs
CC := gcc
CFLAGS := -Wall -Wextra -Werror
all: $(lib)

$(lib): queue.o
	$(STATIC) $@ $^

queue.o: queue.c queue.h 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(lib) $(objects)




