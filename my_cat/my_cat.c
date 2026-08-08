#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Copying the features of cat cli program */
// will contain CLI flags
typedef struct {
  bool number_lines;
  bool number_nonblank_lines;
  bool show_ends;
  bool squeeze_blank;
  bool show_tabs;
  bool show_nonprinting;
} Options;

void print_file(FILE *fp, Options * flag, int * line_number);
FILE * open_file(char *filepath);
int parse_flags(char *argv[], int size, Options *flags); 
void print_help(char * program_name); 
void print_version(char * program_name);

int main(int argc, char *argv[]) {
  int exit_status = 0;
  int line_number = 1;
  int flag_set = 0;
  Options flags;
  
  // initialize flags to false
  flags.number_lines = false;
  flags.number_nonblank_lines = false;
  flags.show_ends = false;
  flags.squeeze_blank = false;
  flags.show_tabs = false;
  flags.show_nonprinting = false;

  flag_set = parse_flags(argv, argc, &flags);
  
  // if no arguments or if only flags as arguments
  if (argc == 1 || (flag_set == (argc - 1))) {
    print_file(stdin, &flags, &line_number);
    return exit_status;
  }

  // opens each file path and displays them one after another
  FILE *fp;
  for (int i = 1; i < argc; i++) {
    // if argument is "-" takes input from stdin until EOF (ctrl+D) then continues  to next argument
    if (strcmp("-", argv[i]) == 0) {
      print_file(stdin, &flags, &line_number); 
    } else if (argv[i][0] == '-') {
        continue;
    }
    else {
      fp = open_file(argv[i]);
      if (fp == NULL) {
        exit_status = 1;
        continue;
      }
      print_file(fp, &flags, &line_number); 
      fclose(fp);
    }
  }
  
  fclose(stdout);
  return exit_status;
}

// displays the contents of fp to stdout
void print_file(FILE *fp, Options * flags, int * line_number) {
  if (fp == NULL) {
    return;
  }

  int c;
  int newline_count = 0;
  bool line_start = true;
  while ((c = fgetc(fp)) != EOF) {

    if (flags->squeeze_blank) {
      if (c == '\n') {
        if (newline_count < 2) {
          newline_count++;
        } else {
          while ((c = fgetc(fp)) == '\n') {
            continue;
          }
          ungetc(c, fp);
          continue;
        }
      }
      else {
        newline_count = 0;
      }
    }
    if (line_start){
      if (flags->number_lines) {
        printf("%6d\t", (*line_number)++);
        line_start = false;
      } else if (flags->number_nonblank_lines && c != '\n') {
        printf("%6d\t",(*line_number)++);
        line_start = false;
      }
    }

    if (c == '\n') {
      line_start = true;
    }

    if (flags->show_ends && (c == 10 || c == 13)) {
      if (c == 10) {
        printf("$\n");
        continue;
      } else if (c == 13) {
        printf("^%c", c + 64);
        continue;
      }
    }
    if (flags->show_tabs && c == 9) {
      printf("^%c", c+64);
      continue;
    }

    if (flags->show_nonprinting && c < 32 && !(c == 9 || c == 10)) {
      printf("^%c", c+64);
      continue;
    }

    fputc(c, stdout);
  }
  
}

FILE * open_file(char *filepath) {
  FILE *fp = fopen(filepath, "r");
  if (fp == NULL) {
    perror(filepath);
    return NULL;
  }
  return fp;
}

// goes through the input and sets and flag in the CL args
// returns a count of number of flags set
int parse_flags(char *argv[], int size, Options *flags) {
  int flag_set = 0;
  int j;
  for (int i = 1; i < size; i++) {
    if (strcmp(argv[i],"-") == 0 || argv[i][0] != '-') {
      // if '-' char means stdin, otherwise if doesn't start with -, then not a flag also ignore
      continue; 
  } else if (argv[i][0] == '-' && (strlen(argv[i]) > 1) && argv[i][1] != '-') {
      j = 1;
      while (argv[i][j] != '\0') {
        switch (argv[i][j]) {
          case 'n':
           flags->number_lines = true;
            break;
          case 'A':
           flags->show_ends = true;
           flags->show_tabs = true;
           flags->show_nonprinting = true;
           break;
          case 'b':
            flags->number_nonblank_lines = true;
            break;
          case 'e':
            flags->show_ends = true;
            flags->show_nonprinting = true;
            break;
          case 'E':
            flags->show_ends = true;
            break;
          case 's':
            flags->squeeze_blank = true;
            break;
          case 't':
            flags->show_nonprinting = true;
            flags->show_tabs = true;
            break;
          case 'T':
            flags->show_tabs = true;
            break;
          case 'v':
            flags->show_nonprinting = true;
            break;
          case 'u':
            break;
          default:
            fprintf(stderr, "my_cat: invalid option -- '%c'\n", argv[i][1]);
            fprintf(stderr, "Try my_cat --help for more information.\n");
            exit(EXIT_FAILURE);
        }
        j++;
      }
      flag_set++;
    }  else {
        
        if (strcmp(argv[i], "--help") == 0) {
          print_help(argv[0]);
        } else if (strcmp(argv[i], "--version") == 0) {
          print_version(argv[0]);
        } else if (strcmp(argv[i], "--number") == 0) {
            flags->number_lines = true;
        } else if (strcmp(argv[i],"--show-all") == 0)  {
            flags->show_ends = true;
            flags->show_tabs = true;
            flags->show_nonprinting = true;
        } else if (strcmp(argv[i], "--number-nonblank") == 0) {
            flags->number_nonblank_lines = true;
        } else if (strcmp(argv[i], "--show-ends") == 0) {
            flags->show_ends = true;
        } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
            flags->squeeze_blank = true;
        } else if (strcmp(argv[i], "--show-tabs") == 0) {
            flags->show_tabs = true;
        } else if (strcmp(argv[i], "--show-nonprinting") == 0) {
            flags->show_nonprinting = true;
        } else {
            fprintf(stderr, "my_cat: invalid option -- '%c'\n", argv[i][1]);
            fprintf(stderr, "Try my_cat --help for more information.\n");
            exit(EXIT_FAILURE);
        }
        flag_set++;
    }
  }
  // -b flag overrides -n
  if (flags->number_nonblank_lines) {
    flags->number_lines = false;
  }
  return flag_set;
}

void print_help(char * program_name) {
  printf("Usage: %s [OPTION]... [FILE]...\n", program_name);
  printf("Concatenate File(s) to standard output.\n");
  printf("\nWith no FILE, or when FILE is -, read standard input.\n");
  printf("\n   %-25sequivalent to -vET\n","-A, --show-all");
  printf("   %-25snumber nonempty output lines, overrides -n\n", "-b, --number-nonblank");
  printf("   %-25sequivalent to -vE\n", "-e");
  printf("   %-25sdisplay $ at end of each line\n", "-E, --show-ends");
  printf("   %-25snumber all output lines\n", "-n, --number");
  printf("   %-25ssuppress repeated empty output lines\n", "-s, --squeeze-blank");
  printf("   %-25sequivalent to -vT\n", "-t");
  printf("   %-25sdisplay TAB characters as ^I\n", "-T, --show-tabs");
  printf("   %-25s(ignored)\n", "-u");
  printf("   %-25suse ^ and M- notation, except for LFD and TAB\n", "-v, --show-nonprinting");
  printf("\t    %-14sdisplay this help and exit\n", "--help");
  printf("\t    %-14soutput version information and exit\n", "--version");

  printf("\nExamples:\n");
  printf("   %s %-3s Output f's contents, then standard input, then g's contents.\n", program_name, "f - g");
  printf("   %-11s Copy standard input to standard output.\n", program_name);

  printf("\nThis is a copy of cat, a GNU coreutils program. It was made to practice programming in C.\n");

  exit(EXIT_SUCCESS);
}

void print_version(char * program_name) {
  printf("%s (clone of GNU coreutils) 9.4", program_name);
  printf("\nNo claim of any copyright or license\n");
  printf("There is NO WARRANTY, to the extent permitted by law.\n");
  printf("\nWritten by Abrasive Squid\n");

  exit(EXIT_SUCCESS);
}
