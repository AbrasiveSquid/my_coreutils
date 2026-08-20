# variables
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
DEBUGFLAGS= -DDEBUG -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
TARGETS = my_ls
DEBUG_TARGETS = my_ls_debug
OBJECTS = my_ls.o my_ls_helper.o
DEBUG_OBJECTS = my_ls_debug.o my_ls_helper_debug.o
HEADERS = my_ls_helper.h debug.h
LS_TEST_TARGETS = ls_no_arg test_layout_ls_no_flags

# build all
all: $(TARGETS)

debug: $(DEBUG_TARGETS)

my_ls: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

my_ls.o: my_ls.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c

my_ls_helper.o: my_ls_helper.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c

my_ls_debug: $(DEBUG_OBJECTS)
	$(CC) $(DEBUGFLAGS) $^ -o $@

my_ls_debug.o: my_ls.c $(HEADERS)
	$(CC) $(DEBUGFLAGS) $< -c -o $@

my_ls_helper_debug.o: my_ls_helper.c $(HEADERS)
	$(CC) $(DEBUGFLAGS) $< -c -o $@

.PHONY: clean $(LS_TEST_TARGETS)

clean:
	rm -f $(TARGETS) $(DEBUG_TARGETS) *.o a.out my_out* sys_out*


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
	@script -q -c "./my_ls" /tmp/my_ls.out
	@script -q -c "ls" /tmp/ls.out
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_multiple_term_widths_ls_no_flags: my_ls
	@echo "test_multiple_term_widths_ls_no_flags"
	@for width in 20 30 40 53 60 79 80 81 100 120 127 160; do \
		echo "testing width $$width..."; \
		script -q -c "stty cols $$width; ./my_ls" /tmp/my_ls.out >/dev/null; \
		script -q -c "stty cols $$width; ls" /tmp/ls.out >/dev/null; \
		expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm; \
		expand -t 8 /tmp/ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm; \
		diff -u /tmp/my_ls.norm /tmp/ls.norm || exit 1; \
	done
	@echo "passed"
