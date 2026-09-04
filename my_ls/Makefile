# variables
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
DEBUGFLAGS= -DDEBUG -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
TARGETS = my_ls
DEBUG_TARGETS = my_ls_debug
OBJECTS = my_ls.o my_ls_helper.o my_ls_printer.o
DEBUG_OBJECTS = my_ls_debug.o my_ls_helper_debug.o
HEADERS = my_ls_helper.h debug.h my_ls_printer.h
COMMON_TESTS = 
MAC_TESTS = ls_no_arg_mac test_no_flags_mac test_show_all_flag_mac test_multiple_paths_mac test_file_path_mac test_invalid_path_mac test_multile_path_with_incorrect_path_mac test_multiple_file_path_mac test_long_list_mac
LINUX_TESTS = ls_no_arg_linux test_no_flags_linux test_show_all_flag_linux test_multiple_paths_linux test_file_path_linux test_invalid_path_linux test_multile_path_with_incorrect_path_linux test_multiple_file_path_linux test_long_list_linux test_hard_link_linux test_hard_link_2digits_linux test_regular_file_linux test_directory_type_linux test_symbolic_link_linux test_fifo_linux test_socket_linux test_character_device_linux test_block_device_linux test_combined_la_linux test_timestamp_cases_linux test_broken_symlink_linux test_mixed_size_device_linux test_zero_values_linux test_empty_directory_linux test_long_multiple_operands_linux
MAC_SLOW_TESTS = test_multiple_term_widths_ls_no_flags_mac 
LINUX_SLOW_TESTS = test_multiple_term_widths_ls_no_flags_linux


# test setup
UNAME_S := $(shell uname -s | tr '[:upper:]' '[:lower:]')
ifeq ($(UNAME_S), linux)
	LS_TEST_TARGETS = $(COMMON_TESTS) $(LINUX_TESTS)
	LS_SLOW_TESTS_TARGETS = $(LINUX_SLOW_TESTS)
else ifeq ($(UNAME_S), darwin) # mac target
	LS_TEST_TARGETS = $(COMMON_TESTS) $(MAC_TESTS)
	LS_SLOW_TESTS_TARGETS = $(MAC_SLOW_TESTS)
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

my_ls_printer.o: my_ls_printer.c $(HEADERS) 
	$(CC) $(CFLAGS) $< -c

my_ls_debug: $(DEBUG_OBJECTS)
	$(CC) $(DEBUGFLAGS) $^ -o $@

my_ls_debug.o: my_ls.c $(HEADERS)
	$(CC) $(DEBUGFLAGS) $< -c -o $@

my_ls_helper_debug.o: my_ls_helper.c $(HEADERS)
	$(CC) $(DEBUGFLAGS) $< -c -o $@

.PHONY: clean $(LS_TEST_TARGETS) ls_tests ls_all_tests $(LS_SLOW_TESTS_TARGETS)

clean:
	rm -f $(TARGETS) $(DEBUG_TARGETS) *.o *.out my_out* sys_out*


# tests
ls_tests: $(LS_TEST_TARGETS) 

ls_all_tests: $(LS_TEST_TARGETS) $(LS_SLOW_TESTS_TARGETS)

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

test_no_flags_linux: my_ls
	@printf "test_no_flags..."
	@script -q -c "./my_ls" /tmp/my_ls.out > /dev/null
	@script -q -c "ls" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_no_flags_mac: my_ls
	@printf "test_no_flags..."
	@script -q  /tmp/my_ls.out ./my_ls> /dev/null
	@script -q /tmp/ls.out gls > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_show_all_flag_linux: my_ls
	@printf "test_show_all_flag_linux..."
	@script -q -c "./my_ls -a" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -a" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c "./my_ls -a ~/" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -a ~/" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/alpha /tmp/my_ls_test/beta /tmp/my_ls_test/.hidden /tmp/my_ls_test/.snapshots /tmp/my_ls_test/zebra
	@script -q -c "./my_ls -a /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -a /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c "./my_ls /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test/
	@echo "passed"

test_show_all_flag_mac: my_ls
	@printf "test_show_all_flag_mac..."
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
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/alpha /tmp/my_ls_test/beta /tmp/my_ls_test/.hidden /tmp/my_ls_test/.snapshots /tmp/my_ls_test/zebra
	@script -q -c /tmp/my_ls.out ./my_ls -a /tmp/my_ls_test> /dev/null
	@script -q -c /tmp/ls.out gls -a /tmp/my_ls_test > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c /tmp/my_ls.out ./my_ls /tmp/my_ls_test> /dev/null
	@script -q -c /tmp/ls.out gls /tmp/my_ls_test > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test/
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
	@echo " passed"

test_multiple_term_widths_ls_no_flags_mac: my_ls
	@for width in {1..400}; do \
	printf "\rtest_multiple_term_widths_ls_no_flags: testing terminal widths 1-400...%3d" $$width; \
		script -q /tmp/my_ls.out sh -c "stty cols $$width; ./my_ls" >/dev/null; \
		script -q /tmp/ls.out sh -c "stty cols $$width; gls" >/dev/null; \
		expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm; \
		expand -t 8 /tmp/ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm; \
		diff -u /tmp/my_ls.norm /tmp/ls.norm || exit 1; \
	done
	@echo " passed"

test_multiple_paths_linux: my_ls
	@printf "test_multiple_paths_linux..."
	@script -q -c "./my_ls ../ ." /tmp/my_ls.out > /dev/null
	@script -q -c "ls ../ ." /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c "./my_ls /home / ../ ." /tmp/my_ls.out > /dev/null
	@script -q -c "ls ../ . /home /" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_multiple_paths_mac: my_ls
	@printf "test_multiple_paths_mac..."
	@script -q  /tmp/my_ls.out ./my_ls ../ . > /dev/null
	@script -q /tmp/ls.out gls ../ . > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_file_path_linux: my_ls
	@printf "test_file_path..."
	@script -q -c "./my_ls ./my_ls.c" /tmp/my_ls.out > /dev/null
	@script -q -c "ls ./my_ls.c" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_file_path_mac: my_ls
	@printf "test_file_path..."
	@script -q  /tmp/my_ls.out ./my_ls ./my_ls.c > /dev/null
	@script -q /tmp/ls.out ls ./my_ls.c > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_multiple_file_path_linux: my_ls
	@printf "test_multiple_file_path..."
	@script -q -c "./my_ls my_ls.c my_ls_helper.c my_ls_helper.h" /tmp/my_ls.out > /dev/null
	@script -q -c "ls my_ls.c my_ls_helper.c my_ls_helper.h" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_multiple_file_path_mac: my_ls
	@printf "test_multiple_file_path..."
	@script -q ./my_ls my_ls_helper.c my_ls_helper.h /tmp/my_ls.out > /dev/null
	@script -q ls my_ls.c my_ls_helper.c my_ls_helper.h /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_multile_path_with_incorrect_path_linux: my_ls
	@printf "test_multile_path_with_incorrect_path..."
	@script -q -c "./my_ls . sdasdasd ../ fakedir /home" /tmp/my_ls.out > /dev/null
	@script -q -c "ls . sdasdasd ../ fakedir /home" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s#^\./my_ls:#ls:#; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_multile_path_with_incorrect_path_mac: my_ls
	@printf "test_multile_path_with_incorrect_path..."
	@script -q ./my_ls . sdasdasd ../ fakedir /home /tmp/my_ls.out > /dev/null
	@script -q ls . sdasdasd ../ fakedir /home /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s#^\./my_ls:#ls:#; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_invalid_path_linux: my_ls
	@printf "test_invalid_path..."
	@script -q -c "./my_ls fakedir " /tmp/my_ls.out > /dev/null
	@script -q -c "ls fakedir" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s#^\./my_ls:#ls:#; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_invalid_path_mac: my_ls
	@printf "test_invalid_path..."
	@script -q ./my_ls fakedir /tmp/my_ls.out > /dev/null
	@script -q ls fakedir /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s#^\./my_ls:#ls:#; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_long_list_linux: my_ls
	@printf "test_long_list..."
	@script -q -c "./my_ls -l" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c "./my_ls -l ~/" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l ~/" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

# need to update to match linux tesdt
test_long_list_mac: my_ls
	@printf "test_long_list..."
	@script -q  /tmp/my_ls.out ./my_ls -l > /dev/null
	@script -q /tmp/ls.out gls -l > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_hard_link_linux: my_ls
	@printf "test_hard_link..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/original
	@touch /tmp/my_ls_test/normal
	@ln /tmp/my_ls_test/original /tmp/my_ls_test/hardlink
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_hard_link_2digits_linux: my_ls
	@printf "test_hard_link..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/original
	@touch /tmp/my_ls_test/normal
	@for i in $$(seq 1 11); do \
		ln /tmp/my_ls_test/original /tmp/my_ls_test/hardlink_$$i; \
	done
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_regular_file_linux: my_ls
	@printf "test_regular_file_linux..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/regular
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_directory_type_linux: my_ls
	@printf "test_directory_type..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test/directory
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_symbolic_link_linux: my_ls
	@printf "test_symbolic_link..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/regular
	@ln -s regular /tmp/my_ls_test/symlink
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"


test_fifo_linux: my_ls
	@printf "test_fifo..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@mkfifo /tmp/my_ls_test/fifo
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_socket_linux: my_ls
	@printf "test_socket..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@python3 -c 'import socket; s = socket.socket(socket.AF_UNIX); s.bind("/tmp/my_ls_test/socket"); s.close()'
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_character_device_linux: my_ls
	@printf "test_character_device..."
	@script -q -c "./my_ls -l /dev/null" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /dev/null" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_block_device_linux: my_ls
	@printf "test_block_device..."
	@block="$$(find /dev -type b -print -quit 2>/dev/null)"; \
	if [ -z "$$block" ]; then \
		echo "skipped (no block device found)"; \
	else \
		script -q -c "./my_ls -l $$block" /tmp/my_ls.out > /dev/null; \
		script -q -c "ls -l $$block" /tmp/ls.out > /dev/null; \
		expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm; \
		expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm; \
		diff -u /tmp/my_ls.norm /tmp/ls.norm; \
		echo "passed"; \
	fi

test_combined_la_linux: my_ls
	@printf "test_combined_la..."
	@script -q -c "./my_ls -la" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -la" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c "./my_ls -la ~/" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -al ~/" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@echo "passed"

test_timestamp_cases_linux: my_ls
	@printf "test_timestamp_cases..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/recent
	@touch -d '7 months ago' /tmp/my_ls_test/old
	@touch -d 'tomorrow' /tmp/my_ls_test/future
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_broken_symlink_linux: my_ls
	@printf "test_broken_symlink..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@ln -s does_not_exist /tmp/my_ls_test/broken
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_mixed_size_device_linux: my_ls
	@printf "test_mixed_size_device..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@truncate -s 123456 /tmp/my_ls_test/regular
	@touch /tmp/my_ls_test/zero
	@script -q -c "./my_ls -l /dev/null /tmp/my_ls_test/regular /tmp/my_ls_test/zero" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /dev/null /tmp/my_ls_test/regular /tmp/my_ls_test/zero" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_zero_values_linux: my_ls
	@printf "test_zero_values..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test
	@touch /tmp/my_ls_test/zero
	@touch -d 'yesterday 18:00:00' /tmp/my_ls_test/zero
	@script -q -c "./my_ls -l /tmp/my_ls_test" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_empty_directory_linux: my_ls
	@printf "test_empty_directory..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test/empty
	@script -q -c "./my_ls -l /tmp/my_ls_test/empty" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test/empty" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@script -q -c "./my_ls -la /tmp/my_ls_test/empty" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -la /tmp/my_ls_test/empty" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"

test_long_multiple_operands_linux: my_ls
	@printf "test_long_multiple_operands..."
	@rm -rf /tmp/my_ls_test
	@mkdir -p /tmp/my_ls_test/directory
	@touch /tmp/my_ls_test/file
	@touch /tmp/my_ls_test/directory/inside
	@script -q -c "./my_ls -l /tmp/my_ls_test/file /tmp/my_ls_test/missing /tmp/my_ls_test/directory" /tmp/my_ls.out > /dev/null
	@script -q -c "ls -l /tmp/my_ls_test/file /tmp/my_ls_test/missing /tmp/my_ls_test/directory" /tmp/ls.out > /dev/null
	@expand -t 8 /tmp/my_ls.out | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s#^\./my_ls:#ls:#; s/[[:space:]]*$$//' > /tmp/my_ls.norm
	@expand -t 8 /tmp/ls.out    | tr -d '\r' | sed '/^Script started/d; /^Script done/d; s/[[:space:]]*$$//' > /tmp/ls.norm
	@diff -u /tmp/my_ls.norm /tmp/ls.norm
	@rm -rf /tmp/my_ls_test
	@echo "passed"
