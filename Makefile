exec = test
sources = $(wildcard ../ihct/src/*.c)
sources += tests/test_linked_list.c
sources += tests/test_slab_allocator.c
objects = $(sources:.c=.o)
LDFLAGS = -lpthread
INCLUDE = -I./include/linked_list
INCLUDE += -I./include/slab_allocator
INCLUDE += -I../ihct/src
CFLAGS = -g -Wall -std=gnu99 $(INCLUDE)
CC = gcc

$(exec): $(objects)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c $(sources)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(exec) src/*.o examples/*.o

.PHONY: clean
