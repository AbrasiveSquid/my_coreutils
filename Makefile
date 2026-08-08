# variables
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -fsanitize=address,undefined
TARGETS = my_cat 
OBJECTS = my_cat.o

#build all programs
all: $(TARGETS)

my_cat: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

my_cat.o: my_cat.c
	$(CC) $(CFLAGS) $< -c

.PHONY: clean cat_leak cat_combined_flags cat_without_newline cat_e_flag cat_number_and_blank cat_no_eof cat_escape_char cat_squeeze_test cat_help cat_show_all cat_show_tabs cat_show_non_print cat_all_flags cat_show_tabs_and_non_printing cat_show_ends cat_show_ends_and_non_printing cat_line_num cat_nonblank_line cat_version


clean:
	rm -f $(TARGETS) *.o a.out my_out* sys_out*

# mem leak test
cat_leak:
	$(CC) $(CFLAGS) -fsanitize=address my_cat.c -o my_cat
	./my_cat ./cat_test_files/cat_test.txt -n

cat_tests: cat_combined_flags cat_without_newline cat_e_flag cat_number_and_blank cat_no_eof cat_escape_char cat_squeeze_test cat_help cat_show_all cat_show_tabs cat_show_non_print cat_all_flags cat_show_tabs_and_non_printing cat_show_ends cat_show_ends_and_non_printing cat_line_num cat_nonblank_line cat_version

cat_combined_flags: my_cat
	./my_cat -ns ./cat_test_files/line_num.txt >my_out 
	cat -ns ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -f my_out sys_out


cat_without_newline: my_cat
	./my_cat ./cat_test_files/eof_after_3newline.txt > my_out
	cat ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	rm -f my_out sys_out
	./my_cat -s ./cat_test_files/eof_after_3newline.txt > my_out 
	cat -s ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	rm -f my_out sys_out

cat_e_flag: my_cat
	./my_cat -e ./cat_test_files/eof_no_newline.txt > my_out
	cat -e ./cat_test_files/eof_no_newline.txt > sys_out
	diff my_out sys_out
	rm -f my_out sys_out

cat_number_and_blank: my_cat
	./my_cat -n -b ./cat_test_files/line_num.txt > my_out
	cat -n -b ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -f my_out sys_out
	./my_cat -b -n ./cat_test_files/line_num.txt > my_outa
	./my_cat -n -b ./cat_test_files/line_num.txt > my_outb
	diff my_outa my_outb 
	rm -f my_outa my_outb

cat_no_eof: my_cat
	./my_cat -vet ./cat_test_files/no_eof.txt > my_out
	cat -vet ./cat_test_files/no_eof.txt > sys_out
	diff my_out sys_out 
	rm -f my_out sys_out

cat_escape_char: my_cat
	./my_cat -vet ./cat_test_files/cat_test.txt > my_out
	cat -vet ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	rm -f my_out sys_out

cat_squeeze_test: my_cat
	./my_cat -s ./cat_test_files/squeeze.txt > my_out
	cat -s ./cat_test_files/squeeze.txt > sys_out
	diff my_out sys_out
	rm -f my_out sys_out

cat_help: my_cat
	./my_cat --help > my_out
	diff my_out ./cat_test_files/help_output.txt
	./my_cat - --help > my_out
	diff my_out ./cat_test_files/help_output.txt
	./my_cat -nsA --help > my_out
	diff my_out ./cat_test_files/help_output.txt
	rm -f my_out


cat_version: my_cat
	./my_cat --version > my_out
	diff my_out ./cat_test_files/version_output.txt
	./my_cat - --version > my_out
	diff my_out ./cat_test_files/version_output.txt
	./my_cat -nsA --version > my_out
	diff my_out ./cat_test_files/version_output.txt
	rm -f my_out

cat_show_all: my_cat
	./my_cat -A ./cat_test_files/help_output.txt > my_out
	cat -A ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -A ./cat_test_files/cat_test.txt > my_out
	cat -A ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -A ./cat_test_files/eof_after_3newline.txt > my_out
	cat -A ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -A ./cat_test_files/line_num.txt > my_out
	cat -A ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_show_tabs: my_cat
	./my_cat -T ./cat_test_files/help_output.txt > my_out
	cat -T ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -T ./cat_test_files/cat_test.txt > my_out
	cat -T ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -T ./cat_test_files/eof_after_3newline.txt > my_out
	cat -T ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -T ./cat_test_files/line_num.txt > my_out
	cat -T ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_show_tabs_and_non_printing: my_cat
	./my_cat -t ./cat_test_files/help_output.txt > my_out
	cat -t ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -t ./cat_test_files/cat_test.txt > my_out
	cat -t ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -t ./cat_test_files/eof_after_3newline.txt > my_out
	cat -t ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -t ./cat_test_files/line_num.txt > my_out
	cat -t ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_show_ends: my_cat
	./my_cat -E ./cat_test_files/help_output.txt > my_out
	cat -E ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -E ./cat_test_files/cat_test.txt > my_out
	cat -E ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -E ./cat_test_files/eof_after_3newline.txt > my_out
	cat -E ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -E ./cat_test_files/line_num.txt > my_out
	cat -E ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_show_ends_and_non_printing: my_cat
	./my_cat -E ./cat_test_files/help_output.txt > my_out
	cat -E ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -E ./cat_test_files/cat_test.txt > my_out
	cat -E ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -E ./cat_test_files/eof_after_3newline.txt > my_out
	cat -E ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -E ./cat_test_files/line_num.txt > my_out
	cat -E ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_show_non_print: my_cat
	./my_cat -v ./cat_test_files/help_output.txt > my_out
	cat -v ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -v ./cat_test_files/cat_test.txt > my_out
	cat -v ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -v ./cat_test_files/eof_after_3newline.txt > my_out
	cat -v ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -v ./cat_test_files/line_num.txt > my_out
	cat -v ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_line_num: my_cat
	./my_cat -n ./cat_test_files/help_output.txt > my_out
	cat -n ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -n ./cat_test_files/cat_test.txt > my_out
	cat -n ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -n ./cat_test_files/eof_after_3newline.txt > my_out
	cat -n ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -n ./cat_test_files/line_num.txt > my_out
	cat -n ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_nonblank_line: my_cat
	./my_cat -b ./cat_test_files/help_output.txt > my_out
	cat -b ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -b ./cat_test_files/cat_test.txt > my_out
	cat -b ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -b ./cat_test_files/eof_after_3newline.txt > my_out
	cat -b ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -b ./cat_test_files/line_num.txt > my_out
	cat -b ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

cat_all_flags: my_cat
	./my_cat -AbeEnstTuv ./cat_test_files/help_output.txt > my_out
	cat -AbeEnstTuv ./cat_test_files/help_output.txt > sys_out
	diff my_out sys_out
	./my_cat -AbeEnstTuv ./cat_test_files/cat_test.txt > my_out
	cat -AbeEnstTuv ./cat_test_files/cat_test.txt > sys_out
	diff my_out sys_out
	./my_cat -AbeEnstTuv ./cat_test_files/eof_after_3newline.txt > my_out
	cat -AbeEnstTuv ./cat_test_files/eof_after_3newline.txt > sys_out
	diff my_out sys_out
	./my_cat -AbeEnstTuv ./cat_test_files/line_num.txt > my_out
	cat -AbeEnstTuv ./cat_test_files/line_num.txt > sys_out
	diff my_out sys_out
	rm -rf my_out sys_out

