# variables
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
TARGETS = my_ls
OBJECTS = my_ls.o my_ls_helper.o
HEADERS = my_ls_helper.h
LS_TEST_TARGETS = ls_no_arg test_layout_ls_no_flags

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


# tests
ls_tests: $(LS_TEST_TARGETS)

ls_no_arg: my_ls
	@echo "ls_no_arg test..."
	@./my_ls > my_out
	@ls > sys_out
	@diff my_out sys_out
	@echo "passed"

test_layout_ls_no_flags: my_ls
	@echo "test_layout_ls_no_flags..."
	@script -q /tmp/my_ls.out ./my_ls
	@script -q /tmp/ls.out ls
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed 's/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out | tr -d '\r' | sed 's/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"
