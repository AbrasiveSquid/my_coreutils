# variables
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
DEBUGFLAGS= -DDEBUG -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
TARGETS = my_ls
DEBUG_TARGETS = my_ls_debug
OBJECTS = my_ls.o my_ls_helper.o
DEBUG_OBJECTS = my_ls_debug.o my_ls_helper_debug.o
HEADERS = my_ls_helper.h debug.h
COMMON_TESTS = 
MAC_TESTS = ls_no_arg_mac test_layout_ls_no_flags_mac test_multiple_term_widths_ls_no_flags_mac 
LINUX_TESTS = ls_no_arg_linux test_layout_ls_no_flags_linux test_multiple_term_widths_ls_no_flags_linux


# test setup
UNAME_S := $(shell uname -s | tr '[:upper:]' '[:lower:]')
ifeq ($(UNAME_S), linux)
	LS_TEST_TARGETS = $(COMMON_TESTS) $(LINUX_TESTS)
else ifeq ($(UNAME_S), darwin) # mac target
	LS_TEST_TARGETS = $(COMMON_TESTS) $(MAC_TESTS)
else
	LS_TEST_TARGETS = $(COMMON_TESTS)
endif

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
	rm -f $(TARGETS) $(DEBUG_TARGETS) *.o *.out my_out* sys_out*


# tests
ls_tests: $(LS_TEST_TARGETS)

ls_no_arg_linux: my_ls
	@printf "ls_no_arg test..."
	@touch my_out sys_out
	@./my_ls > my_out
	@ls > sys_out
	@diff my_out sys_out
	@rm -f my_out sys_out
	@echo "passed"

ls_no_arg_mac: my_ls
	@printf "ls_no_arg test..."
	@touch my_out sys_out
	@./my_ls > my_out
	@gls > sys_out
	@diff my_out sys_out
	@rm -f my_out sys_out
	@echo "passed"

test_layout_ls_no_flags_linux: my_ls
	@printf "test_layout_ls_no_flags..."
	@script -q -c "./my_ls" /tmp/my_ls.out > /dev/null
	@script -q -c "ls" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_layout_ls_no_flags_mac: my_ls
	@printf "test_layout_ls_no_flags..."
	@script -q  /tmp/my_ls.out ./my_ls> /dev/null
	@script -q /tmp/ls.out gls > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_layout_ls_show_all_flag_linux: my_ls
	@printf "test_layout_ls_show_all_flag_linux..."
	@script -q -c "./my_ls -a" /tmp/my_ls.out > /dev/null
	@script -q -c "ls" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c "./my_ls -a ~/" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -a ~/" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_layout_ls_show_all_flag_mac: my_ls
	@printf "test_layout_ls_show_all_flag_mac..."
	@script -q  /tmp/my_ls.out ./my_ls -a > /dev/null
	@script -q /tmp/ls.out gls -a > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q  /tmp/my_ls.out ./my_ls -a ~/ > /dev/null
	@script -q /tmp/ls.out gls -a ~/ > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_multiple_term_widths_ls_no_flags_linux: my_ls
	@for width in {1..400}; do \
	printf "\rtest_multiple_term_widths_ls_no_flags: testing terminal widths 1-400...%3d" $$width; \
		script -q -c "stty cols $$width; ./my_ls" /tmp/my_ls.out >/dev/null; \
		script -q -c "stty cols $$width; ls" /tmp/ls.out >/dev/null; \
		expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm; \
		expand -t 8 /tmp/ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm; \
		diff -u /tmp/my_ls.norm /tmp/ls.norm || exit 1; \
	done
	@echo "passed"

test_multiple_term_widths_ls_no_flags_mac: my_ls
	@for width in {1..400}; do \
	printf "\rtest_multiple_term_widths_ls_no_flags: testing terminal widths 1-400...%3d" $$width; \
		script -q /tmp/my_ls.out sh -c "stty cols $$width; ./my_ls" >/dev/null; \
		script -q /tmp/ls.out sh -c "stty cols $$width; gls" >/dev/null; \
		expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm; \
		expand -t 8 /tmp/ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm; \
		diff -u /tmp/my_ls.norm /tmp/ls.norm || exit 1; \
	done
	@echo "passed"
