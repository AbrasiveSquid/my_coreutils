# variables
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
TARGETS = my_ls
OBJECTS = my_ls.o my_ls_helper.o
HEADERS = my_ls_helper.h
# LS_TEST_TARGETS = 

# build all
all: $(TARGETS)

my_ls: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

my_ls.o: my_ls.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c

my_ls_helper.o: my_ls_helper.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c

.PHONY: clean $(LS_TEST_TARGETS)

clean:
	rm -f $(TARGETS) *.o a.out my_out* sys_out*
