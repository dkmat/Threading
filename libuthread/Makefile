# Target library
objects := queue.o uthread.o context.o sem.o preempt.o
lib := libuthread.a
STATIC := ar rcs
CC := gcc
CFLAGS := -Wall -Wextra -Werror
all: $(objects) $(lib) 

$(lib): $(objects)
	$(STATIC) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(lib) $(objects)

